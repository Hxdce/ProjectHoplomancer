// Fill out your copyright notice in the Description page of Project Settings.


#include "./BaseAmmoItem.h"

#include "../PlayerCharacter/PlayerCharacter.h"


ABaseAmmoItem::ABaseAmmoItem()
{
}


bool ABaseAmmoItem::HandlePickupItem(AActor* OtherActor)
{
	APlayerCharacter* p = Cast<APlayerCharacter>(OtherActor);
	if (p != nullptr && p->IsAlive && AmmoType != AMMO_NULL) {
		ABaseWeapon* heldWeapon = p->CurrWeapon;
		// Make this go over all the weapons the player is carrying instead of the one they're actively using!
		bool notCarryingMaxAmmo = heldWeapon->GetTotalAmmoCount() < heldWeapon->AmmoMax;
		if (heldWeapon != nullptr && heldWeapon->AmmoType == this->AmmoType && notCarryingMaxAmmo) {
			return true;
		}
	}
	return false;
}


void ABaseAmmoItem::Activate(AActor* OtherActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("AmmoAmount's value is: %i"), AmmoAmount));
	APlayerCharacter* p = Cast<APlayerCharacter>(OtherActor);
	if (p != nullptr && p->IsAlive) {
		ABaseWeapon* heldWeapon = p->CurrWeapon;
		if (heldWeapon != nullptr) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Adding AmmoAmount's value to %s"), *heldWeapon->PrintName));
			heldWeapon->GiveAmmo(AmmoAmount);
		}
	}
	Super::Activate(OtherActor);
}
