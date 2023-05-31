// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Includes from project code:
#include "../Actors/BaseGameCharacter.h"
#include "../PlayerCharacter/PlayerCharacter.h"

// This include always comes last:
#include "BaseNPC.generated.h"


// The base NPC class. Specifically built for humanoid-style NPCs.

UCLASS()
class PROJECTHOPLOMANCER_API ABaseNPC : public ABaseGameCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	double LastAttackTime;
	double DeathCleanupTime;

public:	
	// Amount of points to give the player on death.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	int PlayerScorePointsValue;

	// Primary attack cooldown time.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	double AttackPrimaryCooldownTime;

	// Primary attack damage.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	float AttackPrimaryDamage;

	// Primary attack range.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	float AttackPrimaryRange;

	// Primary attack physics impulse.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	float AttackPhysicsImpulse;


	// Overridden built-in UE5 class functions:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Built-in function override for taking damage.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	// Overridden project code class functions:

	// ...


	// Proprietary functions below:

	UFUNCTION(BlueprintCallable)
	virtual void PrimaryAttack();

	// Used for deleting the remains of an NPC after death, e.g. its ragdoll or static death pose.
	virtual void DeathCleanup();

	// Function for handling NPC death.
	virtual void HandleDeath(float DamageAmount=0.0f, AController* EventInstigator=nullptr, double PhysicsImpulse=0.0, AActor* DamageCauser=nullptr);
};
