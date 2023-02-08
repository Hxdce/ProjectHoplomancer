// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

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

	virtual void PrimaryAttack();
	virtual void SecondaryAttack();

	virtual float GetNextFireTime();
	virtual void SetNextFireTime(double time);

	virtual void SetThirdPersonMeshVisibility(bool vis);
	virtual bool GetThirdPersonMeshVisibility();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Mesh for model.
	UPROPERTY(EditAnywhere, Category=Mesh)
	UStaticMeshComponent* ThirdPersonMesh;

	// Next time this weapon can fire.
	double NextFireTime;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Variables for the weapon fire rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=WeaponStats)
	float Firerate;

};
