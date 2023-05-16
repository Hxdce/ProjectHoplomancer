// Fill out your copyright notice in the Description page of Project Settings.


#include "./BaseGameCharacter.h"


// Sets default values
ABaseGameCharacter::ABaseGameCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void ABaseGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ABaseGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void ABaseGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


int ABaseGameCharacter::GetHealth()
{
	return CurrentHealth;
}


void ABaseGameCharacter::SetHealth(int amount)
{
	if (amount <= 0)
	{
		Die();
	}
	else
	{
		CurrentHealth = amount;
	}
}


int ABaseGameCharacter::GetMaxHealth()
{
	return MaxHealth;
}


void ABaseGameCharacter::Heal(int healAmount)
{
	if (IsAlive)
	{
		CurrentHealth = FGenericPlatformMath::Min(CurrentHealth + healAmount, MaxHealth);
	}
}


void ABaseGameCharacter::Die()
{
	if (IsAlive)
	{
		IsAlive = false;
		if (CurrentHealth != 0)
		{
			CurrentHealth = 0;
		}
	}
}
