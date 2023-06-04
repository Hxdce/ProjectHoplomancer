// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.


#include "./NPCShockDrone.h"

ANPCShockDrone::ANPCShockDrone()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxHealth = 25;
	CurrentHealth = MaxHealth;
	IsAlive = true;
}
