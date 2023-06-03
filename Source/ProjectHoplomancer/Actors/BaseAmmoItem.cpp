// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.


#include "./BaseAmmoItem.h"


ABaseAmmoItem::ABaseAmmoItem()
{
}


bool ABaseAmmoItem::HandlePickupItem(APlayerCharacter* PlayerActor)
{
	APlayerCharacter* p = PlayerActor;
	if (p != nullptr && p->IsAlive && AmmoType != AMMO_NULL)
	{
		for (ABaseWeapon* weapon : p->GetCarriedWeapons())
		{
			bool notCarryingMaxAmmo = weapon->GetTotalAmmoCount() < weapon->AmmoMax;
			if (weapon != nullptr && weapon->AmmoType == this->AmmoType && notCarryingMaxAmmo)
			{
				TargetWeapon = weapon;
				return true;
			}
		}
	}
	return false;
}


void ABaseAmmoItem::Activate(APlayerCharacter* PlayerActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("AmmoAmount's value is: %i"), AmmoAmount));
	APlayerCharacter* p = PlayerActor;
	if (p != nullptr && p->IsAlive)
	{
		if (TargetWeapon != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Adding AmmoAmount's value to %s"), *TargetWeapon->PrintName));
			TargetWeapon->GiveAmmo(AmmoAmount);
		}
	}
	Super::Activate(PlayerActor);
}
