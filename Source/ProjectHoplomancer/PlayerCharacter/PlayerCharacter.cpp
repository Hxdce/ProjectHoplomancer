// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.


#include "./PlayerCharacter.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Kismet/KismetMathLibrary.h"

#include "../ProjectHoplomancerGameModeBase.h"
#include "../Actors/BaseWeapon.h"


// Sets default values.
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a first person camera component.
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	check(PlayerCamera != nullptr);
	// Attach the camera component to our root component.
	PlayerCamera->SetupAttachment(GetRootComponent());
	// Position the camera above the eyes.
	PlayerCamera->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	// Enable the pawn to control camera rotation.
	PlayerCamera->bUsePawnControlRotation = true;

	GetMesh()->SetOwnerNoSee(true);

	MaxHealth = 100;
	CurrentHealth = MaxHealth;
	IsAlive = true;
	CurrWeaponIndex = -1;

	// Camera recoil stuff.
	CameraRotMod = FRotator(0.0, 0.0, 0.0);
	CameraRotModLastFrame = FRotator(0.0, 0.0, 0.0);
	CameraRecoilVelocity = FRotator(0.0, 0.0, 0.0);
	CameraRecoilDamping = 9.0;
	CameraRecoilSpringMagnitude = 65.0;
}


// Called when the game starts or when spawned.
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			EnhancedInputSubsystem = subsystem;
			EnhancedInputSubsystem->AddMappingContext(PlayerInputMappingContext, 0);
		}
	}

	check(GEngine != nullptr);
	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Using PlayerCharacter class."));
}


// Called every frame.
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, FString::Printf(TEXT("CameraRotMod.Pitch: %f\nCameraRotMod.Yaw: %f\nCameraRotMod.Roll: %f"), CameraRotMod.Pitch, CameraRotMod.Yaw, CameraRotMod.Roll));
	//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, FString::Printf(TEXT("CameraRotMaxPitchChange: %f"), CameraRotMaxPitchChange));

	DecayCameraRecoilRotation(DeltaTime);

	APlayerController* pc = Cast<APlayerController>(Controller);
	if (pc != nullptr)
	{
		if (CameraRotMod.Pitch > CameraRotMaxPitchChange)
		{
			CameraRotMaxPitchChange = CameraRotMod.Pitch;
		}
		// Subtract the previous frame's rotation alteration so we don't stack.
		pc->RotationInput += CameraRotMod - CameraRotModLastFrame;
		// Store for the next frame.
		CameraRotModLastFrame = CameraRotMod;
	}
}


// Called to bind functionality to input.
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PrimaryAttack);
		EnhancedInputComponent->BindAction(SecondaryAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SecondaryAttack);
		EnhancedInputComponent->BindAction(SelectWeaponAction, ETriggerEvent::Started, this, &APlayerCharacter::SelectWeapon);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ReloadWeapon);
	}

}


// Built-in function override for taking damage.
float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!IsAlive)
	{
		return 0.0f;
	}
	
	float res = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	FRotator damageRecoil = FRotator(
		FMath::FRandRange(3.0, 1.0), FMath::FRandRange(-1.0, 1.0), FMath::FRandRange(-1.0, 1.0)
	);
	damageRecoil *= FMath::Min(DamageAmount * 0.2, 10.0);

	CameraApplyRecoil(damageRecoil);

	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0)
	{
		Die();
	}

	return res;
}


// Function for adding a physics impulse to the player, e.g. being pushed by explosions.
void APlayerCharacter::AddPhysicsImpulse(FVector ImpulseOrigin, float Magnitude)
{
	FVector impulse = GetActorLocation() - ImpulseOrigin;
	impulse.Normalize();
	impulse *= Magnitude;
	//GetMovementComponent()->Velocity = impulse;
	LaunchCharacter(impulse, false, false);
}


// Function to handle picking up a weapon.
bool APlayerCharacter::TakeWeapon(ABaseWeapon* wpn)
{
	bool tookSuccessfully = false;
	if (!IsAlive)
	{
		// Can't take anything if you're dead!
		return tookSuccessfully;
	}

	if (CarriedWeapons.Num() < 3 && CarriedWeapons.Find(wpn) == INDEX_NONE)
	{
		// Only pick it up if we're not carrying it.
		// Todo: If we ARE carrying it, take its ammo instead?
		CarriedWeapons.Add(wpn);
		tookSuccessfully = SwitchToWeapon(CarriedWeapons.Num() - 1);
	}

	if (tookSuccessfully)
	{
		// Attach it to the actor, disable collision, and make its third person mesh invisible.
		wpn->SetActorEnableCollision(false);
		wpn->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
		wpn->SetThirdPersonMeshVisibility(false);
		// Create the HUD pickup history message:
		FString message = FString::Printf(TEXT("Picked up a %s"), *wpn->PrintName);
		OnPlayerItemPickup.Broadcast(message);
	}
	return tookSuccessfully;
}


bool APlayerCharacter::SwitchToWeapon(int wpnIndex)
{
	
	bool switchedSuccessfully = false;
	if (IsAlive && !CarriedWeapons.IsEmpty() && wpnIndex < CarriedWeapons.Num())
	{
		if (CurrWeapon != nullptr)  // Already have a weapon equipped. Unequip it.
		{
			CurrWeapon->Unequip();
		}
		CurrWeapon = CarriedWeapons[wpnIndex];  // Switch out the pointer with the new one at the given index.
		if (CurrWeapon != nullptr)
		{
			CurrWeaponIndex = wpnIndex;
			CurrWeapon->Equip();
			CurrWeapon->SetWielder(this);
			switchedSuccessfully = true;
		}
	}

	if (switchedSuccessfully)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("CurrWeapon is now %s!"), *CurrWeapon->GetName()));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CurrWeapon assignment in SwitchToWeapon failed!"));
	}

	return IsAlive && switchedSuccessfully;
}


TArray<ABaseWeapon*> APlayerCharacter::GetCarriedWeapons()
{
	return CarriedWeapons;
}


void APlayerCharacter::Die_Implementation()
{
	if (IsAlive)
	{
		IsAlive = false;
		if (CurrentHealth != 0)
		{
			CurrentHealth = 0;
		}

		//UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
		//MovementComponent->SetMovementMode(MOVE_None);
		//GetCapsuleComponent()->SetCapsuleHalfHeight(GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 4);
		// Probably just control the camera from blueprints for now!

		// Triggers the Player Death event for usage in the player's event graph:
		ReceivePlayerDeath();

		// Broadcasts the OnPlayerDeath delegate for usage with any other actors binded to it.
		OnPlayerDeath.Broadcast();
	}
}


void APlayerCharacter::CameraApplyRecoil(FRotator RecoilRotator, double Snappiness)
{
	Snappiness = FMath::Clamp(Snappiness, 0.0, 1.0);
	CameraRotMod += RecoilRotator * Snappiness;
	CameraRecoilVelocity += RecoilRotator * 20.0 * (1.0 - Snappiness);
}


// The main player movement function.
void APlayerCharacter::Move(const FInputActionValue& Value)
{	
	if (!IsAlive)
	{
		return;
	}

	// Directional character movement. Reacts according to the camera's rotation.
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}


// The main player looking function.
void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}


// For calculating where projectiles originate from and what direction they should face.
void APlayerCharacter::CalculateMuzzlePointOfAim(FVector* OutMuzzleLocation, FVector* OutMuzzleDirection)
{
	// Camera location is the actor's location + the camera's current location relative to its parent.
	*OutMuzzleLocation = GetActorLocation() + PlayerCamera->GetRelativeLocation();  // + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	// Calculate the muzzle direction from the camera's rotation.
	*OutMuzzleDirection = GetViewRotation().Vector();
}


// Handle and decay any camera recoiling effects e.g. from weapon recoil, injury, landing from falls, etc.
void APlayerCharacter::DecayCameraRecoilRotation(float DeltaTime)
{
	// Todo: Make this use vector math instead of rotator math. Yaw alteration decreases when looking more vertical!
	if (!CameraRotMod.IsNearlyZero(0.001) || !CameraRecoilVelocity.IsNearlyZero(0.001))
	{
		CameraRotMod += CameraRecoilVelocity * DeltaTime;

		double damping = FMath::Max(1.0 - (CameraRecoilDamping * DeltaTime), 0.0);  // Don't go below zero.
		CameraRecoilVelocity *= damping;

		double springForceMagnitude = CameraRecoilSpringMagnitude * DeltaTime;
		springForceMagnitude = FMath::Clamp(springForceMagnitude, 0.0, 2.0);
		CameraRecoilVelocity -= CameraRotMod * springForceMagnitude;

		// Clamp the value range so it doesn't wrap around and cause weirdness.
		CameraRotMod = FRotator(
			FMath::Clamp(CameraRotMod.Pitch, -89.0, 89.0),
			FMath::Clamp(CameraRotMod.Yaw, -179.0, 179.0),
			FMath::Clamp(CameraRotMod.Roll, -89.0, 89.0)
		);
	}
	else
	{
		CameraRotMod = FRotator(0.0, 0.0, 0.0);
		CameraRecoilVelocity = FRotator(0.0, 0.0, 0.0);
	}
}
// Credits to Valve Software - this system is heavily based off of how the camera recoil effects
// work in the Source engine for games such as Half-Life 2 and Counter Strike: Source.
// https://www.valvesoftware.com/en/
// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/shared/gamemovement.cpp#L1213


// Primary attack!
void APlayerCharacter::PrimaryAttack(const FInputActionValue& Value)
{
	if (!IsAlive || CurrWeapon == nullptr)
	{
		return;
	}

	// Point of aim stuff.
	FVector MuzzleLocation;
	FVector MuzzleDirection;
	CalculateMuzzlePointOfAim(&MuzzleLocation, &MuzzleDirection);

	CurrWeapon->PrimaryAttack(this, MuzzleLocation, MuzzleDirection);
}


// Secondary attack!
void APlayerCharacter::SecondaryAttack(const FInputActionValue& Value)
{
	if (!IsAlive || CurrWeapon == nullptr)
	{
		return;
	}

	// Point of aim stuff.
	FVector MuzzleLocation;
	FVector MuzzleDirection;
	CalculateMuzzlePointOfAim(&MuzzleLocation, &MuzzleDirection);
	CurrWeapon->SecondaryAttack(this, MuzzleLocation, MuzzleDirection);
}


void APlayerCharacter::SelectWeapon(const FInputActionValue& Value)
{
	if (EnhancedInputSubsystem == nullptr)
	{
		// How???
		return;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Invoking SelectWeapon."));

	APlayerController* pc = Cast<APlayerController>(Controller);
	TArray<FKey> keys = EnhancedInputSubsystem->QueryKeysMappedToAction(SelectWeaponAction);

	int index = 0;
	for (FKey key : keys)
	{
		if (pc->IsInputKeyDown(key) && index != CurrWeaponIndex)
		{
			// Only try to switch if the index differs from the current!
			FString keyPressed = key.ToString();
			SwitchToWeapon(index);
			break;
		}
		index++;
	}
}


void APlayerCharacter::ReloadWeapon(const FInputActionValue& Value)
{
	if (!IsAlive)
	{
		return;
	}

	if (CurrWeapon != nullptr)
	{
		CurrWeapon->ReloadStart();
	}
}