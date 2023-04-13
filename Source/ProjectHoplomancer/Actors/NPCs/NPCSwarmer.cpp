// Fill out your copyright notice in the Description page of Project Settings.


#include "./NPCSwarmer.h"

ANPCSwarmer::ANPCSwarmer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxHealth = 50;
	CurrentHealth = MaxHealth;
	IsAlive = true;
}