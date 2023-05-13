// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseGameCharacter.generated.h"

// The base game character class. 
// This is used for functions, variables, and behaviors shared between the player and NPCs.

UCLASS()
class PROJECTHOPLOMANCER_API ABaseGameCharacter : public ACharacter
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


	// Proprietary functions below:

	// Health and death related functions:
	UFUNCTION(BlueprintCallable)
	virtual int GetHealth();

	UFUNCTION(BlueprintCallable)
	virtual void SetHealth(int amount);

	UFUNCTION(BlueprintCallable)
	virtual int GetMaxHealth();

	UFUNCTION(BlueprintCallable)
	virtual void Heal(int healAmount);

	UFUNCTION(BlueprintCallable)
	virtual void Die();

};
