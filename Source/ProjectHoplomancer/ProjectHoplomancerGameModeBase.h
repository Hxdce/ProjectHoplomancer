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
};
