// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InterfaceKillablePawn.generated.h"


UINTERFACE()
class PROJECTHOPLOMANCER_API UKillablePawn : public UInterface
{
	GENERATED_BODY()
};

class IKillablePawn
{
	GENERATED_BODY()

public:
	// Health and death related functions:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int GetHealth();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetHealth(int amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int GetMaxHealth();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Heal(int healAmount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Die();
};

