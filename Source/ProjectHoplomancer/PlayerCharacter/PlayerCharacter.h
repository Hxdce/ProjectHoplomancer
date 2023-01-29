// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Header includes:
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

// This include stays at the bottom:
#include "PlayerCharacter.generated.h"


// Forward declarations:
class UInputMappingContext;
class UInputAction;


UCLASS()
class PROJECTHOPLOMANCER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputMappingContext* PlayerCharacterContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
