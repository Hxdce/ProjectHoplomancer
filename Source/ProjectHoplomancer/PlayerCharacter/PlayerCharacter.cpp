// Fill out your copyright notice in the Description page of Project Settings.


#include "./PlayerCharacter.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


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

	// Dev variables.
	DevProjectileFirerate = 0.25f;
	DevUseDevGun = false;

	MaxHealth = 100;
	CurrentHealth = MaxHealth;
	IsAlive = true;

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
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerCharacterContext, 0);
		}
	}

	check(GEngine != nullptr);
	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Using PlayerCharacter class."));
}


// Called every frame.
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, FString::Printf(TEXT("CameraRotMod.Pitch: %f\nCameraRotMod.Yaw: %f\nCameraRotMod.Roll: %f"), CameraRotMod.Pitch, CameraRotMod.Yaw, CameraRotMod.Roll));
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, FString::Printf(TEXT("CameraRotMaxPitchChange: %f"), CameraRotMaxPitchChange));

	DecayCameraRecoilRotation(DeltaTime);

	APlayerController* pc = GetLocalViewingPlayerController();
	if (pc != nullptr) {
		if (CameraRotMod.Pitch > CameraRotMaxPitchChange) {
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

	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0)
	{
		Die();
	}

	return res;
}


// Function for adding a physics impulse to the player, e.g. being pushed by explosions.
void APlayerCharacter::AddPhysicsImpulse(FVector ImpulseOrigin, float magnitude)
{
	FVector impulse = GetActorLocation() - ImpulseOrigin;
	impulse.Normalize();
	impulse *= magnitude;
	//GetMovementComponent()->Velocity = impulse;
	LaunchCharacter(impulse, false, false);
}


// Function to handle picking up a weapon.
bool APlayerCharacter::TakeWeapon(ABaseWeapon* wpn)
{
	CurrWeapon = wpn;
	if (!IsAlive || CurrWeapon == nullptr)
	{
		// Operation failed somehow.
		return false;
	}
	CurrWeapon->SetActorEnableCollision(false);
	CurrWeapon->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
	CurrWeapon->SetThirdPersonMeshVisibility(false);
	CurrWeapon->SetWielder(this);
	return true;
}


void APlayerCharacter::Heal(int healAmount)
{
	if (IsAlive)
	{
		CurrentHealth = FGenericPlatformMath::Min(CurrentHealth + healAmount, MaxHealth);
	}
}


void APlayerCharacter::Die()
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
void APlayerCharacter::CalculateMuzzlePointOfAim(FVector* OutMuzzleLocation, FRotator* OutMuzzleRotation)
{
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	FVector f = PlayerCamera->GetRelativeLocation();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Camera: %f %f %f"), f.X, f.Y, f.Z));
	// Set MuzzleOffset to spawn projectiles where the camera is located, accounting for offset when crouching.
	MuzzleOffset.Set(0.0f, 0.0f, f.Z - BaseEyeHeight);

	// Transform MuzzleOffset from camera space to world space.
	*OutMuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	// Get the camera rotation as the muzzle rotation.
	*OutMuzzleRotation = CameraRotation;
}


// Handle and decay any camera recoiling effects e.g. from weapon recoil, injury, landing from falls, etc.
void APlayerCharacter::DecayCameraRecoilRotation(float DeltaTime)
{
	if (!CameraRotMod.IsNearlyZero(0.001) || !CameraRecoilVelocity.IsNearlyZero(0.001)) {
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
	else {
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
	FRotator MuzzleRotation;
	CalculateMuzzlePointOfAim(&MuzzleLocation, &MuzzleRotation);

	// Dev code for testing firing projectiles.
	if (DevUseDevGun)
	{
		// Can't fire faster than weapon firerate.
		if (GetWorld()->GetTimeSeconds() < DevProjectileNextFireTime)
		{
			return;
		}

		if (DevProjectileClass)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SP;
				SP.Owner = this;
				SP.Instigator = GetInstigator();

				ABaseProjectile* Projectile = World->SpawnActor<ABaseProjectile>(DevProjectileClass, MuzzleLocation, MuzzleRotation, SP);
				if (Projectile)
				{
					// Set the projectile's initial trajectory.
					FVector LaunchDirection = MuzzleRotation.Vector();
					Projectile->FireInDirection(LaunchDirection);
				}
				DevProjectileNextFireTime = GetWorld()->GetTimeSeconds() + DevProjectileFirerate;
			}
		}
	}
	else
	{
		//FString out = FString::Printf(TEXT("Attempting primary attack with %s!"), *CurrWeapon->GetClass()->GetName());
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, out);
		CurrWeapon->PrimaryAttack(this, MuzzleLocation, MuzzleRotation);
	}
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
	FRotator MuzzleRotation;
	CalculateMuzzlePointOfAim(&MuzzleLocation, &MuzzleRotation);
	CurrWeapon->SecondaryAttack(this, MuzzleLocation, MuzzleRotation);
}


void APlayerCharacter::ReloadWeapon(const FInputActionValue& Value)
{
	if (!IsAlive)
	{
		return;
	}

	if (CurrWeapon != nullptr)
	{
		CurrWeapon->ReloadWeapon(false);
	}
}