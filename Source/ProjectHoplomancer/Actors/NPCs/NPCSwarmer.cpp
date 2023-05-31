// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.


#include "./NPCSwarmer.h"

ANPCSwarmer::ANPCSwarmer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxHealth = 50;
	CurrentHealth = MaxHealth;
	IsAlive = true;
}