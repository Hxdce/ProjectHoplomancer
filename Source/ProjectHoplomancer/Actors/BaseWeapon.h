// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Includes from project code:
#include "../Enums/WeaponEnums.h"
#include "./BaseProjectile.h"
#include "../ProjectHoplomancerGameModeBase.h"

// This include always comes last:
#include "BaseWeapon.generated.h"


// Forward declarations:
class USoundBase;


// Custom delegate signatures:
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponFireSignature, class AActor*, Instigator, FVector, Location);


UCLASS()
class PROJECTHOPLOMANCER_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties.
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Mesh for model.
	UPROPERTY(EditAnywhere, Category=Mesh)
	UStaticMeshComponent* ThirdPersonMesh;

	// Collider for pickup detection.
	UPROPERTY(VisibleDefaultsOnly, Category=Collision)
	class USphereComponent* PickupCollider;

	// Next time this weapon can fire.
	double NextFireTime;

	// Time this weapon can reload. Prevents reloading in the middle of firing.
	double TimeCanReload;

	// Whether a reload is queued or not. This typically happens when trying to reload while firing.
	bool QueuedReload;

	// Variable for the amount of ammo in the weapon's reservoir (magazine, cylinder, hopper, etc).
	int ReservoirCurrRoundCount;

	// Variable for the current total amount of ammo being carried.
	int TotalAmmoCount;
	// Ammo is tied to individual weapon instances, like in Halo or FEAR.

	// Boolean for whether the weapon is reloading or not.
	bool IsReloading;

	// Boolean for whether the weapon is equipped or not.
	bool IsEquipped;

	// Character wielding the weapon.
	ACharacter* Wielder;

	// Primary attack SFX.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponSounds)
	USoundBase* SoundPrimaryAttack;

	// Dry fire SFX.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponSounds)
	USoundBase* SoundDryFire;

	// Finish reload SFX.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponSounds)
	USoundBase* SoundFinishReload;

	// Fired Projectile.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	TSubclassOf<class ABaseProjectile> WeaponProjectile;

	// ---
	// Weapon Spread stuff:

	// The current accuracy penalty saturation and recovery time from recoil effects, in seconds.
	UPROPERTY(EditAnywhere, Category=Camera)
	double WeaponSpreadRecoilPenaltyTime;

	// The accuracy penalty from player movement.
	UPROPERTY(EditAnywhere, Category=Camera)
	double WeaponSpreadMovementPenalty;

	// Sum of the held weapon's inherent accuracy + recoil and movement penalties.
	UPROPERTY(EditAnywhere, Category=Camera)
	double WeaponSpreadFinal;

public:
	// Variable for the damage of the primary fire mode.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	float DamagePrimary;

	// Variable for the damage of the secondary fire mode.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	float DamageSecondary;

	// Spread value for the fired projectile(s). This is the best possible accuracy this weapon can achieve.
	// Represents the diameter of the cone of fire from the player's POV, in degrees.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	double WeaponAccuracy;

	// The maximum amount of accuracy penalty (spread) from weapon recoil when the player's time value reaches its max.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	double RecoilSpreadMax;

	// The maximum recovery time when the accuracy penalty value reaches its max.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	double RecoilSpreadTimeMax;

	// How much time is added to the accuracy penalty value per shot.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	double RecoilSpreadTimePerShot;

	// Variable for the weapon's fire rate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	float Firerate;

	// Variable for the projectile velocity of the weapon.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	float ProjectileVelocity;

	// Variable for how long the weapon takes to reload.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	float ReloadDuration;

	// Delay time for reloading after firing.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	float ReloadFireDelayTime;

	// Variable for the minimum vertical recoil of the weapon.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	float RecoilPitchMin;

	// Variable for the maximum vertical recoil of the weapon.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	float RecoilPitchMax;

	// Variable for the minimum horizontal recoil of the weapon.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	float RecoilYawMin;

	// Variable for the maximum horizontal recoil of the weapon.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	float RecoilYawMax;

	// Variable for how snappy the weapon's recoil is.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	float RecoilSnappiness;

	// Variable for the weapon's max ammo reservoir (e.g. magazine, cylinder, hopper) capacity.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	int ReservoirMax;

	// Variable for the maximum amount of ammo (reservoir + extra) this weapon can carry.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	int AmmoMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=WeaponStats)
	TEnumAsByte<WeaponAmmoTypes> AmmoType;

	// Variable for the weapon's printed name on UIs and such.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	FString PrintName;


	// Overridden built-in UE5 class functions:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;


	// Proprietary functions:


	// Equip this weapon.
	virtual void Equip();

	// Unequip this weapon. This cancels any actions the weapon is currently doing, e.g. reloading.
	virtual void Unequip();

	// Primary attack function.
	virtual void PrimaryAttack(AActor* Parent, FVector MuzzleLocation, FVector MuzzleDirection);

	// Secondary attack function.
	virtual void SecondaryAttack(AActor* Parent, FVector MuzzleLocation, FVector MuzzleDirection);

	// ^^^
	// Might be wise to remove the "Parent" parameter from the attack functions soon,
	// since having the "Wielder" member variable makes it redundant!

	// This is used to add the baseline random spread to a fired projectile, based on its WeaponAccuracy value.
	// This is independent of any penalties and represents the best possible accuracy of the weapon.
	// WeaponAccuracy represents the diameter of the cone of fire.
	virtual void AddBaselineSpreadToProjectile(FVector* FiringDirection);

	// This is used to add spread penalties to the baseline cone of fire of the weapon, i.e. a cone of fire
	// within a cone of fire essentially. The reason we do this independent of a weapon's baseline spread is
	// so weapons that fire multiple projectiles at once e.g. shotguns won't have their baseline weapon
	// spread altered, only where within the penalty cone of fire that baseline spread goes.
	virtual void AddWeaponSpreadPenalties(FVector* FiringDirection);

	// Get next time we can fire.
	virtual float GetNextFireTime();

	// Set next time we can fire.
	virtual void SetNextFireTime(double Time);

	// Get whether the third person mesh is visible or not.
	virtual bool GetThirdPersonMeshVisibility();

	// Set whether the third person mesh is visible or not.
	virtual void SetThirdPersonMeshVisibility(bool Vis);

	// Get the wielder of this weapon.
	virtual ACharacter* GetWielder();

	// Set the wielder of this weapon.
	virtual void SetWielder(ACharacter* NewWielder);

	virtual void ReloadStart();

	virtual void ReloadFinish();

	virtual void DevReloadFinishSound();  // Probably gonna change how this works at some point!

	UFUNCTION(BlueprintCallable)
	virtual double CalculateWeaponSpreadRecoilPenalty();

	void AddWeaponSpreadRecoilPenaltyTime(float Multiplier=1.0);

	void DecayWeaponSpreadRecoilPenaltyTime(float DeltaTime);

	double GetWeaponSpreadRecoilPenaltyTime();

	// Gets the total amount of spread in degrees for the current weapon, penalties and all.
	UFUNCTION(BlueprintCallable)
	virtual double GetWeaponSpreadTotal();

	virtual void ApplyRecoil();

	// Getter for the current reservoir round count.
	UFUNCTION(BlueprintCallable)
	int GetCurrentReservoirRoundCount();

	// Setter for the current reservoir round count.
	UFUNCTION(BlueprintCallable)
	void SetCurrentReservoirRoundCount(int count);

	// Getter for the current total ammo count.
	UFUNCTION(BlueprintCallable)
	int GetTotalAmmoCount();

	// Leaving this out for now since it's tricky to set up correctly!
	// Setter for the current total ammo count.
	//UFUNCTION(BlueprintCallable)
	//void SetTotalAmmoCount(int count);

	// Give ammo to this weapon.
	UFUNCTION(BlueprintCallable)
	void GiveAmmo(int AmmoAmount);

	// Events:

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Weapon Fire"))
	void ReceiveWeaponFire(AActor* WeaponParent, FVector Location);

	// Delegates:

	// Delegate when firing.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="EventDispatchers")
	FWeaponFireSignature OnWeaponFire;
};
