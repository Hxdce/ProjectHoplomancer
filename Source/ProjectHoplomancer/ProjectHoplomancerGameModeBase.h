// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectHoplomancerGameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTHOPLOMANCER_API AProjectHoplomancerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void StartPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	int GetPlayerScore();

	UFUNCTION(BlueprintCallable)
	void SetPlayerScore(int n);

	UFUNCTION(BlueprintCallable)
	void AddToPlayerScore(int n);

	// Default values for variables have to be assigned in
	// the header since gamemodes don't use a constructor.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Performance)
	float CorpseRagdollCleanupTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	float ProjectilePhysicsImpulseMultiplier = 1.0f;

	// Increases the physics impulse above for the killing hit on an NPC.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	float NPCDeathPhysicsImpulseMultiplier = 3.0f;
protected:
	int PlayerScore = 0;
};
