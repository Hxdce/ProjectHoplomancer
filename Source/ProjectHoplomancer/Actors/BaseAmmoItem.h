// Fill out your copyright notice in the Description page of Project Settings.

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
	virtual bool HandlePickupItem(AActor* OtherActor) override;

	// Function for what the item does when picked up.
	virtual void Activate(AActor* OtherActor) override;
};
