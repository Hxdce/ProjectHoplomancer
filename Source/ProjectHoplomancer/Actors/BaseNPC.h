// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Includes from project code:
#include "../ProjectHoplomancerGameModeBase.h"

// This include always comes last:
#include "BaseNPC.generated.h"

UCLASS()
class PROJECTHOPLOMANCER_API ABaseNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float DeathCleanupTime;

	// Current health.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	int CurrentHealth;

	// Maximum health.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	int MaxHealth;

	// Character is alive?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	bool IsAlive;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Built-in function override for taking damage.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Amount of points to give the player on death.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	int PlayerScorePointsValue;

	// Proprietary functions below:

	UFUNCTION(BlueprintCallable)
	void PrimaryAttack();

	// Used for deleting the remains of an NPC after death, e.g. its ragdoll or static death pose.
	void DeathCleanup();

	UFUNCTION(BlueprintCallable)
	void Heal(int healAmount);

	UFUNCTION(BlueprintCallable)
	void Die(AController* EventInstigator=nullptr, AActor* DamageCauser=nullptr, float DamageAmount=0.0f);

};
