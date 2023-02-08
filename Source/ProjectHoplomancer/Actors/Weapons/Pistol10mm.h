// A 10mm Pistol.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWeapon.h"

// This include always comes last:
#include "Pistol10mm.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHOPLOMANCER_API APistol10mm : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APistol10mm();

	virtual void PrimaryAttack() override;

	virtual void SecondaryAttack() override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
