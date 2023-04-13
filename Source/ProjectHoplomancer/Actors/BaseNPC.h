// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "TimerManager.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool IsAlive;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Used for deleting the remains of an NPC after death, e.g. its ragdoll or static death pose.
	void DeathCleanup();

	UFUNCTION(BlueprintCallable)
	void InflictDamage(int damageAmount);

	UFUNCTION(BlueprintCallable)
	void Heal(int healAmount);

	UFUNCTION(BlueprintCallable)
	void Die();
};
