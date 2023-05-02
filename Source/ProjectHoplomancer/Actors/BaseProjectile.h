// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"

// Includes from project code:
#include "../ProjectHoplomancerGameModeBase.h"

// This include always comes last:
#include "BaseProjectile.generated.h"

UCLASS()
class PROJECTHOPLOMANCER_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Collider component.
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* Collider;

	// Projectile movement.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	class UProjectileMovementComponent* MovementComponent;

	// Actor that gets credit for firing this projectile.
	UPROPERTY()
	TObjectPtr<class AController> ProjectileFirer;

	// Damage value.
	UPROPERTY(EditAnywhere)
	float DamageValue;

	// Function to invoke when hitting something.
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Function that initializes the projectile to shoot in a given direction.
	void FireInDirection(const FVector& ShootDirection);
};
