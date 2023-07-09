// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWeapon.h"

// This include always comes last:
#include "ShotgunPumpAction.generated.h"

// The Pump-Action Shotgun. Fires 12 gauge 000 magnum buckshot.
UCLASS()
class PROJECTHOPLOMANCER_API AShotgunPumpAction : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AShotgunPumpAction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Shell insert SFX.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponSounds)
	USoundBase* SoundInsertShell;

	// Whether firing is queued or not. This will stop other actions like reloading.
	bool QueuedFiring;

public:
	// Variable for the number of projectiles the weapon fires.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=WeaponStats)
	int ProjectileCount;


	// Overridden built-in UE5 class functions:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Overridden project code class functions:

	// This is used to add random spread to a fired projectile, creating the weapon's cone of fire.
	// This is independent of any penalties and represents the best possible accuracy of the weapon.
	virtual void AddBaselineSpreadToProjectile(FVector* FiringDirection);

	// Proprietary functions below:

	virtual void PrimaryAttack(AActor* Parent, FVector MuzzleLocation, FVector MuzzleDirection) override;

	virtual void SecondaryAttack(AActor* Parent, FVector MuzzleLocation, FVector MuzzleDirection) override;

	virtual void ReloadStart() override;

	virtual void ReloadFinish() override;


	// Proprietary functions below:

	virtual void ReloadInsertShell();
};
