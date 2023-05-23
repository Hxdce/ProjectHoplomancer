// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "../Interfaces/InterfaceKillablePawn.h"

#include "BaseGamePawn.generated.h"

// The base game pawn class. 
// This is used for the functions, variables, and behaviors shared between NON-HUMANOID pawns.

UCLASS()
class PROJECTHOPLOMANCER_API ABaseGamePawn : public APawn, public IKillablePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseGamePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	// Health and death related functions:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int GetHealth();
	virtual int GetHealth_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetHealth(int amount);
	virtual void SetHealth_Implementation(int amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int GetMaxHealth();
	virtual int GetMaxHealth_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Heal(int healAmount);
	virtual void Heal_Implementation(int healAmount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Die();
	virtual void Die_Implementation();

};
