// The player character.

#pragma once

// Header includes:
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "TimerManager.h"

// Includes from project code:
#include "../Actors/BaseProjectile.h"
#include "../Actors/BaseWeapon.h"
#include "../ProjectHoplomancerGameModeBase.h"

// This include always comes last:
#include "PlayerCharacter.generated.h"


// Forward declarations:
class UInputMappingContext;
class UInputAction;


// Custom delegate signatures:
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDeathSignature)


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

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* ReloadAction;

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

	// Weapons the player is carrying.
	TArray<ABaseWeapon*> Weapons;

	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	// Current health.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	int CurrentHealth;

	// Maximum health.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	int MaxHealth;

	// Proprietary Functions:

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void PrimaryAttack(const FInputActionValue& Value);
	void SecondaryAttack(const FInputActionValue& Value);
	void ReloadWeapon(const FInputActionValue& Value);
	void CalculateMuzzlePointOfAim(FVector* OutMuzzleLocation, FRotator* OutMuzzleRotation);

public:
	// The current weapon the player is using (if any).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	ABaseWeapon* CurrWeapon;

	// Character is alive?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	bool IsAlive;

	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Built-in function override for taking damage.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Proprietary functions below:

	// Function to handle picking up a weapon.
	virtual bool TakeWeapon(ABaseWeapon* wpn);

	UFUNCTION(BlueprintCallable)
	void Heal(int healAmount);

	UFUNCTION(BlueprintCallable)
	void Die();


	// Events:

	// Player death event, for using in the PLAYER'S EVENT GRAPH.
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Player Death"))
	void ReceivePlayerDeath();

	// Delegates:

	// On Player death delegate, for binding to OTHER THINGS.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="EventDispatchers")
	FPlayerDeathSignature OnPlayerDeath;
};
