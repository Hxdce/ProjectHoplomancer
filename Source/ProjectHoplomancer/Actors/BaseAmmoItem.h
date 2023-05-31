// � Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.

#pragma once

#include "CoreMinimal.h"
#include "./BasePickupableItem.h"

// Includes from project code:
#include "./BaseWeapon.h"

// This include always comes last:
#include "BaseAmmoItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHOPLOMANCER_API ABaseAmmoItem : public ABasePickupableItem
{
	GENERATED_BODY()

public:
	ABaseAmmoItem();

	using Super = ABasePickupableItem;

protected:

public:
	// The amount of ammo this item has.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=ItemStats)
	int AmmoAmount;

	// The type of ammo this item has.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=ItemStats)
	TEnumAsByte<WeaponAmmoTypes> AmmoType;

	// Function for checking whether the item meets its necessary criteria for being picked up.
	// E.g. for a healing item, this would check if the player is injured.
	virtual bool HandlePickupItem(APlayerCharacter* PlayerActor) override;

	// Function for what the item does when picked up.
	virtual void Activate(APlayerCharacter* PlayerActor) override;
};
