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


// Note to self: It might be better to refactor APlayerCharacter and ABaseNPC at some point so
// they inherit from the same class! Some of their member functions & variables can be shared.


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

	// ---
	// Components:

	// Player's camera.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Camera)
	UCameraComponent* PlayerCamera;
	// Player's mesh.
	UPROPERTY(EditAnywhere, Category=Mesh)
	UStaticMeshComponent* PlayerStaticMesh;

	// ---------
	
	
	// ---
	// Stuff for player controls:

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
	
	// ---------
	

	// ---
	// Dev gun stuff:

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
	
	// ---------
	

	// ---
	// Camera recoil stuff:

	// Camera rotation modification for recoil.
	FRotator CameraRotMod;
	// Camera rotation modification for recoil on the previous frame.
	FRotator CameraRotModLastFrame;
	// Camera recoil velocity. Resets to zero over time.
	FRotator CameraRecoilVelocity;
	
	double CameraRotMaxPitchChange = 0;

	// ---------
	
	// ---
	// Other:

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
	void DecayCameraRecoilRotation(float DeltaTime);

public:
	// The current weapon the player is using (if any).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	ABaseWeapon* CurrWeapon;

	// Character is alive?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	bool IsAlive;

	// Higher values increase the amount of damping with camera recoil.
	UPROPERTY(EditAnywhere, Category=Camera)
	double CameraRecoilDamping;
	// Higher values increases the speed at which the recoil decays.
	UPROPERTY(EditAnywhere, Category=Camera)
	double CameraRecoilSpringMagnitude;

	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Built-in function override for taking damage.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Proprietary functions below:

	// Function for adding a physics impulse to the player, e.g. being pushed by explosions.
	UFUNCTION(BlueprintCallable)
	void AddPhysicsImpulse(FVector ImpulseOrigin, float magnitude);

	// Function to handle picking up a weapon.
	virtual bool TakeWeapon(ABaseWeapon* wpn);

	// Health and death related functions:
	UFUNCTION(BlueprintCallable)
	int GetHealth();

	UFUNCTION(BlueprintCallable)
	void SetHealth(int amount);

	UFUNCTION(BlueprintCallable)
	int GetMaxHealth();

	UFUNCTION(BlueprintCallable)
	void Heal(int healAmount);

	UFUNCTION(BlueprintCallable)
	void Die();

	// Function for applying camera recoil effects.
	void CameraApplyRecoil(FRotator RecoilRotator, double Snappiness=0.0);

	// Events:

	// Player death event, for using in the PLAYER'S EVENT GRAPH.
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Player Death"))
	void ReceivePlayerDeath();

	// Delegates:

	// On Player death delegate, for binding to OTHER THINGS.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="EventDispatchers")
	FPlayerDeathSignature OnPlayerDeath;
};
