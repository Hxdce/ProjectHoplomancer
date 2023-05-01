// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Includes from project code:
#include "../BaseNPC.h"

// This include always comes last:
#include "LogicNPCSpawner.generated.h"

UCLASS()
class PROJECTHOPLOMANCER_API ALogicNPCSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALogicNPCSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	double NextSpawnTime;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Trigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Logic)
	TSubclassOf<ABaseNPC> NPCToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Logic)
	bool IsEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Logic)
	double SpawnTimeInterval;
};
