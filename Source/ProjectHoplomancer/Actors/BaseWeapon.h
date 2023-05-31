// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Includes from project code:
#include "./BaseProjectile.h"
#include "../ProjectHoplomancerGameModeBase.h"

// This include always comes last:
#include "BaseWeapon.generated.h"


// Forward declarations:
class USoundBase;


// Custom delegate signatures:
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponFireSignature, class AActor*, Instigator, FVector, Location)


// Enum for ammo types.
UENUM(BlueprintType)
enum WeaponAmmoTypes {
	AMMO_NULL,
	AMMO_10MM,
	AMMO_12GAUGE,
	AMMO_57SMG
};


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
	double WeaponSpread;

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

	virtual void PrimaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation);

	virtual void SecondaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation);

	// ^^^
	// Might be wise to remove the "Parent" parameter from the attack functions soon,
	// since having the "Wielder" member variable makes it redundant!

	// This is used to add random spread to a fired projectile, creating the weapon's cone of fire.
	// The member variable "WeaponSpread" defines the diameter of this cone from the player's POV in degrees.
	virtual void AddSpreadToProjectile(FVector* FiringDirection);

	virtual float GetNextFireTime();

	virtual void SetNextFireTime(double Time);

	virtual bool GetThirdPersonMeshVisibility();

	virtual void SetThirdPersonMeshVisibility(bool Vis);

	virtual ACharacter* GetWielder();

	virtual void SetWielder(ACharacter* NewWielder);

	virtual void ReloadStart();

	virtual void ReloadFinish();

	virtual void DevReloadFinishSound();  // Probably gonna change how this works at some point!

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
