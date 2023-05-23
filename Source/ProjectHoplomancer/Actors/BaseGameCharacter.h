// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "../Interfaces/InterfaceKillablePawn.h"

#include "BaseGameCharacter.generated.h"

// The base game character class. 
// This is used for the functions, variables, and behaviors shared between HUMANOID pawns.
// E.g. the player and certain NPCs.

UCLASS()
class PROJECTHOPLOMANCER_API ABaseGameCharacter : public ACharacter, public IKillablePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseGameCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Current health.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	int CurrentHealth;

	// Maximum health.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	int MaxHealth;

public:	
	// Character is alive?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	bool IsAlive;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Interface functions below:

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
