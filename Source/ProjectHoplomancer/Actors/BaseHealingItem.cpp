// Fill out your copyright notice in the Description page of Project Settings.


#include "./BaseHealingItem.h"

#include "../PlayerCharacter/PlayerCharacter.h"

ABaseHealingItem::ABaseHealingItem()
{

}


bool ABaseHealingItem::HandlePickupItem(AActor* OtherActor)
{
	APlayerCharacter* p = Cast<APlayerCharacter>(OtherActor);
	// Return true if cast was successful, player is alive, and player health is less than max.
	return p != nullptr && p->IsAlive && p->GetHealth() < p->GetMaxHealth();
}


void ABaseHealingItem::Activate(AActor* OtherActor)
{
	APlayerCharacter* p = Cast<APlayerCharacter>(OtherActor);
	if (p != nullptr)
	{
		p->Heal(HealAmount);
	}
	Super::Activate(OtherActor);
}