// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"

// Includes from project code:
#include "./BaseProjectile.h"
#include "../ProjectHoplomancerGameModeBase.h"

// This include always comes last:
#include "BaseWeapon.generated.h"


// Custom delegate signatures:
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponFireSignature, class AActor*, Instigator, FVector, Location)


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

	// Variable for the amount of ammo in the weapon's reservoir (magazine, cylinder, hopper, etc).
	int ReservoirCurrRoundCount;

	// Variable for the current total amount of ammo being carried.
	int TotalAmmoCount;
	// Ammo is tied to individual weapon instances, like in Halo or FEAR.

	// Character wielding the weapon.
	ACharacter* Wielder;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Overriden functions:

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Proprietary functions:

	virtual void PrimaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation);
	virtual void SecondaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation);

	virtual float GetNextFireTime();
	virtual void SetNextFireTime(double Time);

	virtual bool GetThirdPersonMeshVisibility();
	virtual void SetThirdPersonMeshVisibility(bool Vis);

	virtual ACharacter* GetWielder();
	virtual void SetWielder(ACharacter* NewWielder);

	virtual void ReloadWeapon(bool EmptyReload = false);

	virtual void ApplyRecoil();

	// Variable for the damage of the primary fire mode.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	float DamagePrimary;

	// Variable for the damage of the secondary fire mode.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	float DamageSecondary;

	// Variable for the weapon's fire rate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	float Firerate;

	// Variable for the projectile velocity of the weapon.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	float ProjectileVelocity;

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

	// Variable for the weapon's printed name on UIs and such.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	FString PrintName;

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

	// Events:

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Weapon Fire"))
	void ReceiveWeaponFire(AActor* WeaponParent, FVector Location);

	// Delegates:

	// Delegate when firing.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="EventDispatchers")
	FWeaponFireSignature OnWeaponFire;
};
