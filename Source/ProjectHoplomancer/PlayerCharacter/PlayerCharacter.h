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
#include "../Actors/BaseGameCharacter.h"

// This include always comes last:
#include "PlayerCharacter.generated.h"


// Forward declarations:
class UInputMappingContext;
class UInputAction;
class ABaseWeapon;
class UEnhancedInputLocalPlayerSubsystem;


// Custom delegate signatures:
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerItemPickupSignature, FString, StringToPrint);

UCLASS()
class PROJECTHOPLOMANCER_API APlayerCharacter : public ABaseGameCharacter
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
	UInputMappingContext* PlayerInputMappingContext;

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
	UInputAction* SelectWeaponAction;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* ReloadAction;
	
	// Enhanced Input Subsystem:
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem;

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

	// Weapons the player is carrying, both equipped and unequipped.
	TArray<ABaseWeapon*> CarriedWeapons;
	// Current index of the equipped weapon.
	int CurrWeaponIndex;

	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	// Proprietary Functions:

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void PrimaryAttack(const FInputActionValue& Value);
	void SecondaryAttack(const FInputActionValue& Value);
	void SelectWeapon(const FInputActionValue& Value);
	void ReloadWeapon(const FInputActionValue& Value);
	void CalculateMuzzlePointOfAim(FVector* OutMuzzleLocation, FRotator* OutMuzzleRotation);
	void DecayCameraRecoilRotation(float DeltaTime);

public:
	// The current weapon the player is using (if any).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	ABaseWeapon* CurrWeapon;

	// Higher values increase the amount of damping with camera recoil.
	UPROPERTY(EditAnywhere, Category=Camera)
	double CameraRecoilDamping;
	// Higher values increases the speed at which the recoil decays.
	UPROPERTY(EditAnywhere, Category=Camera)
	double CameraRecoilSpringMagnitude;


	// Overridden built-in UE5 class functions:

	// Called every frame.
	void Tick(float DeltaTime) override;

	// Called to bind functionality to input.
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Built-in function override for taking damage.
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	// Overridden project code class functions:

	void Die_Implementation() override;


	// Proprietary functions below:

	// Function for adding a physics impulse to the player, e.g. being pushed by explosions.
	UFUNCTION(BlueprintCallable)
	void AddPhysicsImpulse(FVector ImpulseOrigin, float Magnitude);

	// Function to handle picking up a weapon.
	bool TakeWeapon(ABaseWeapon* wpn);

	// Function to switch to a weapon.
	bool SwitchToWeapon(int wpnIndex);

	// Function for getting all the weapons the player is carrying, equipped or otherwise.
	TArray<ABaseWeapon*> GetCarriedWeapons();

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

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="EventDispatchers")
	FPlayerItemPickupSignature OnPlayerItemPickup;
};
