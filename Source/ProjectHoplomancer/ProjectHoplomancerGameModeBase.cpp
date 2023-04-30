// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectHoplomancerGameModeBase.h"


void AProjectHoplomancerGameModeBase::StartPlay()
{
	Super::StartPlay();

	check(GEngine != nullptr)
	{
		// Display a debug message for five seconds. 
		// The -1 "Key" value argument prevents the message from being updated or refreshed.
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is ProjectHoplomancerGameModeBase!"));
	}

	PlayerScore = 0;
}

int AProjectHoplomancerGameModeBase::GetPlayerScore()
{
	return PlayerScore;
}

void AProjectHoplomancerGameModeBase::SetPlayerScore(int n)
{
	PlayerScore = n;
}

void AProjectHoplomancerGameModeBase::AddToPlayerScore(int n)
{
	PlayerScore += n;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Adding to player's score!!!"));
}
