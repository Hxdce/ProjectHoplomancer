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

protected:
	int PlayerScore;
};
