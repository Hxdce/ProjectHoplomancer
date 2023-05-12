// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BasePickupableItem.h"

// This include always comes last:
#include "BaseHealingItem.generated.h"


UCLASS()
class PROJECTHOPLOMANCER_API ABaseHealingItem : public ABasePickupableItem
{
	GENERATED_BODY()
	
public:

	ABaseHealingItem();

	using Super = ABasePickupableItem;

protected:

public:
	// The amount of health this item restores.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=ItemStats)
	int HealAmount;

	// Function for checking whether the item meets its necessary criteria for being picked up.
	// E.g. for a healing item, this would check if the player is injured.
	virtual bool HandlePickupItem(APlayerCharacter* PlayerActor) override;

	// Function for what the item does when picked up.
	virtual void Activate(APlayerCharacter* PlayerActor) override;
};