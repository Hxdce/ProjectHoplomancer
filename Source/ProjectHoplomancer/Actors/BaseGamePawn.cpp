// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.


#include "./BaseGamePawn.h"


// Sets default values
ABaseGamePawn::ABaseGamePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void ABaseGamePawn::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ABaseGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void ABaseGamePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


int ABaseGamePawn::GetHealth_Implementation()
{
	return 0;
}


void ABaseGamePawn::SetHealth_Implementation(int amount)
{
}


int ABaseGamePawn::GetMaxHealth_Implementation()
{
	return 0;
}


void ABaseGamePawn::Heal_Implementation(int healAmount)
{
}


void ABaseGamePawn::Die_Implementation()
{
}
