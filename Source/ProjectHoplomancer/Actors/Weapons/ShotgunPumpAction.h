// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWeapon.h"

// This include always comes last:
#include "ShotgunPumpAction.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHOPLOMANCER_API AShotgunPumpAction : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AShotgunPumpAction();

	virtual void PrimaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation) override;

	virtual void SecondaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Fired Projectile.
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ABaseProjectile> WeaponProjectile;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Variable for the number of projectiles the weapon fires.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	int ProjectileCount;

};
