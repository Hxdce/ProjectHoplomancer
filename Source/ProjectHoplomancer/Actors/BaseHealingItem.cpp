// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.


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