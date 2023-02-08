// The player character.

#pragma once

// Header includes:
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// Includes from project code:
#include "../Actors/BaseProjectile.h"
#include "../Actors/BaseWeapon.h"

// This include always comes last:
#include "PlayerCharacter.generated.h"


// Forward declarations:
class UInputMappingContext;
class UInputAction;


UCLASS()
class PROJECTHOPLOMANCER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Player Controls.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Camera)
	UCameraComponent* PlayerCamera;

	UPROPERTY(EditAnywhere, Category=Mesh)
	UStaticMeshComponent* PlayerStaticMesh;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputMappingContext* PlayerCharacterContext;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* PrimaryAttackAction;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* SecondaryAttackAction;


	// Dev Projectile.
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ABaseProjectile> DevProjectileClass;
	// Dev Projectile Firerate.
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	double DevProjectileFirerate;
	// Next time player can fire.
	double DevProjectileNextFireTime;
	// Bool for controlling whether the dev gun is active.
	bool DevUseDevGun;

	// The current weapon the player is using (if any).
	ABaseWeapon* CurrWeapon;

	// Weapons the player is carrying.
	TArray<ABaseWeapon*> Weapons;

	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;


	// Proprietary Functions:

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void PrimaryAttack(const FInputActionValue& Value);
	void SecondaryAttack(const FInputActionValue& Value);
	void CalculateMuzzlePointOfAim(FVector* OutMuzzleLocation, FRotator* OutMuzzleRotation);

public:	
	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Function to handle picking up a weapon.
	virtual bool TakeWeapon(ABaseWeapon* wpn);
};
