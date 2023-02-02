// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

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

	// Damage value.
	UPROPERTY(EditAnywhere)
	float DamageValue = 8.0f;

	// Function to invoke when hitting something.
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& Hit);

	// Function that initializes the projectile to shoot in a given direction.
	void FireInDirection(const FVector& ShootDirection);
};
