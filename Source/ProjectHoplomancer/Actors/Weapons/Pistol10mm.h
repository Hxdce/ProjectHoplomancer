// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWeapon.h"

// This include always comes last:
#include "Pistol10mm.generated.h"

// A standard pistol. Fires 10x21mm auto rounds.
UCLASS()
class PROJECTHOPLOMANCER_API APistol10mm : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APistol10mm();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PrimaryAttack(AActor* Parent, FVector MuzzleLocation, FVector MuzzleDirection) override;

	virtual void SecondaryAttack(AActor* Parent, FVector MuzzleLocation, FVector MuzzleDirection) override;
};
