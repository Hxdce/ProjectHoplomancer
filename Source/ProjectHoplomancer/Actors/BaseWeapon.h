// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"

// Includes from project code:
#include "./BaseProjectile.h"

// This include always comes last:
#include "BaseWeapon.generated.h"

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

	// Variable for the weapon's fire rate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	float Firerate;

	// Variable for the weapon's max ammo reservoir (e.g. magazine, cylinder, hopper) capacity.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	int ReservoirMax;

	// Variable for the weapon's current ammo count.
	int ReservoirCurrRoundCount;
};
