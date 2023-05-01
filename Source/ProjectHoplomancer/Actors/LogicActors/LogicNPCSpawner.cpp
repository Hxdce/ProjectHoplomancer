// Fill out your copyright notice in the Description page of Project Settings.


#include "./LogicNPCSpawner.h"

// Sets default values
ALogicNPCSpawner::ALogicNPCSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnTimeInterval = 3.0;  // 3 seconds by default.
}


// Called when the game starts or when spawned
void ALogicNPCSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void ALogicNPCSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsEnabled) {
		return;
	}

	if (NextSpawnTime <= GetWorld()->GetTimeSeconds()) {
		Trigger();
		NextSpawnTime = GetWorld()->GetTimeSeconds() + SpawnTimeInterval;
	}
}


void ALogicNPCSpawner::Trigger()
{
	ABaseNPC* SpawnedNPC = GetWorld()->SpawnActor<ABaseNPC>(NPCToSpawn, GetActorLocation(), GetActorRotation());
	
}

