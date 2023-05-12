// Fill out your copyright notice in the Description page of Project Settings.


#include "./BaseHealingItem.h"


ABaseHealingItem::ABaseHealingItem()
{

}


bool ABaseHealingItem::HandlePickupItem(APlayerCharacter* PlayerActor)
{
	APlayerCharacter* p = PlayerActor;
	// Return true if pointer isn't null, player is alive, and player health is less than max.
	return p != nullptr && p->IsAlive && p->GetHealth() < p->GetMaxHealth();
}


void ABaseHealingItem::Activate(APlayerCharacter* PlayerActor)
{
	APlayerCharacter* p = PlayerActor;
	if (p != nullptr && p->IsAlive)
	{
		p->Heal(HealAmount);
	}
	Super::Activate(PlayerActor);
}