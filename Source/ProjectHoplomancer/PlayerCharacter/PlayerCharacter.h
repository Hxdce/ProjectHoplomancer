// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Header includes:
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// Includes from project code:
#include "../Actors/BaseProjectile.h"

// This include always comes last:
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

	void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Player Controls.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Camera)
	UCameraComponent* PlayerCamera;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputMappingContext* PlayerCharacterContext;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* CrouchStartAction;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* CrouchStopAction;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* PrimaryAttackAction;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* SecondaryAttackAction;


	// Used for smooth crouching.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Crouch)
	FVector CrouchEyeOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Crouch)
	float CrouchSpeed;


	// Dev Projectile.
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ABaseProjectile> DevProjectileClass;
	// Dev Projectile Firerate.
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	double DevProjectileFirerate;

	double DevProjectileNextFireTime;

	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartCrouch(const FInputActionValue& Value);
	void StopCrouch(const FInputActionValue& Value);
	void PrimaryAttack(const FInputActionValue& Value);
	void SecondaryAttack(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
