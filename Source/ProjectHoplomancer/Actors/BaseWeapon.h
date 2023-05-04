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

	// Overriden functions:

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Proprietary functions:

	virtual void PrimaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation);
	virtual void SecondaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation);

	virtual float GetNextFireTime();
	virtual void SetNextFireTime(double Time);

	virtual void SetThirdPersonMeshVisibility(bool Vis);
	virtual bool GetThirdPersonMeshVisibility();

	virtual void ReloadWeapon(bool EmptyReload=false);

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

	// Variable for the weapon's max ammo reservoir (e.g. magazine, cylinder, hopper) capacity.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	int ReservoirMax;

	// Variable for the weapon's current ammo count.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=WeaponStats)
	int ReservoirCurrRoundCount;

	// Variable for the weapon's printed name on UIs and such.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	FString PrintName;


	// Events:

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Weapon Fire"))
	void ReceiveWeaponFire(AActor* WeaponParent, FVector Location);

	// Delegates:

	// Delegate when firing.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="EventDispatchers")
	FWeaponFireSignature OnWeaponFire;
};
