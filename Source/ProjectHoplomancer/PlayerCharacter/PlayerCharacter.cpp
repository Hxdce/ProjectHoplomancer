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
	// Position the camera slightly above the eyes.
	PlayerCamera->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	// Enable the pawn to control camera rotation.
	PlayerCamera->bUsePawnControlRotation = true;

	// Used for smooth crouching.
	CrouchEyeOffset = FVector(0.0f);
	CrouchSpeed = 12.0f;

	// Dev variables.
	DevProjectileFirerate = 0.25f;
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


// Called to bind functionality to input.
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(CrouchStartAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchStopAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StopCrouch);
		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PrimaryAttack);
		EnhancedInputComponent->BindAction(SecondaryAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SecondaryAttack);
	}

}


// The main player movement function.
void APlayerCharacter::Move(const FInputActionValue& Value)
{	
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


// Start crouching...
void APlayerCharacter::StartCrouch(const FInputActionValue& Value)
{
	Crouch();
}


// Stop crouching...
void APlayerCharacter::StopCrouch(const FInputActionValue& Value)
{
	UnCrouch();
}


// OnStartCrouch override.
void APlayerCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (HalfHeightAdjust == 0.0f)
	{
		return;
	}

	float StartBaseEyeHeight = BaseEyeHeight;
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	CrouchEyeOffset.Z += StartBaseEyeHeight - BaseEyeHeight + HalfHeightAdjust;
	PlayerCamera->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight), false);
}


// OnEndCrouch override.
void APlayerCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (HalfHeightAdjust == 0.0f)
	{
		return;
	}

	float StartBaseEyeHeight = BaseEyeHeight;
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	CrouchEyeOffset.Z += StartBaseEyeHeight - BaseEyeHeight - HalfHeightAdjust;
	PlayerCamera->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight), false);
}


// CalcCamera override.
void APlayerCharacter::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	if (PlayerCamera)
	{
		PlayerCamera->GetCameraView(DeltaTime, OutResult);
		OutResult.Location += CrouchEyeOffset;
	}
}


// Primary attack!
void APlayerCharacter::PrimaryAttack(const FInputActionValue& Value)
{
	// Dev code for testing firing projectiles.

	// Can't fire faster than weapon firerate.
	if (GetWorld()->GetTimeSeconds() < DevProjectileNextFireTime)
	{
		return;
	}

	if (DevProjectileClass)
	{
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
		MuzzleOffset.Set(50.0f, 0.0f, 0.0f);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		// Get the camera rotation as the muzzle rotation.
		FRotator MuzzleRotation = CameraRotation;


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
				DevProjectileNextFireTime = GetWorld()->GetTimeSeconds() + DevProjectileFirerate;
			}
		}
	}
}


// Secondary attack!
void APlayerCharacter::SecondaryAttack(const FInputActionValue& Value)
{
}


// Called every frame.
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CrouchInterpolationTime = FMath::Min(1.0f, CrouchSpeed * DeltaTime);
	CrouchEyeOffset = (1.0f - CrouchInterpolationTime) * CrouchEyeOffset;
}