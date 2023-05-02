// Fill out your copyright notice in the Description page of Project Settings.


#include "./BaseNPC.h"

// Sets default values
ABaseNPC::ABaseNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Some default values for NPCs.
	MaxHealth = 100;
	CurrentHealth = MaxHealth;
	IsAlive = true;
	PlayerScorePointsValue = 100;
}


// Called when the game starts or when spawned
void ABaseNPC::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ABaseNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void ABaseNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


float ABaseNPC::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!IsAlive)
	{
		return 0.0f;
	}
	
	float res = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0 && IsAlive)
	{
		Die(EventInstigator, DamageCauser, DamageAmount);
	}

	return res;
}


void ABaseNPC::PrimaryAttack()
{
}


void ABaseNPC::DeathCleanup()
{
	Destroy();
}


void ABaseNPC::Heal(int healAmount)
{
	CurrentHealth = FGenericPlatformMath::Min(CurrentHealth + healAmount, MaxHealth);
}


void ABaseNPC::Die(AController* EventInstigator, AActor* DamageCauser, float DamageAmount)
{
	if (IsAlive)
	{
		AProjectHoplomancerGameModeBase* GameMode = Cast<AProjectHoplomancerGameModeBase>(GetWorld()->GetAuthGameMode());

		if (EventInstigator != nullptr) {
			FString out = FString::Printf(TEXT("NPC death causer is %s!"), *EventInstigator->GetPawn()->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, out);
			ACharacter* inst = Cast<ACharacter>(EventInstigator->GetPawn());
			if (inst == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) {
				// Only change the score if the event instigator for this NPC's death was the player.
				if (GameMode != nullptr)
				{
					GameMode->AddToPlayerScore(PlayerScorePointsValue);
				}
				else {
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("AProjectHoplomancerGameModeBase cast failed in BaseNPC!!!"));
				}
			}
		}
		
		USkeletalMeshComponent* NPCMesh = GetMesh();
		UCapsuleComponent* NPCCapsuleComponent = GetCapsuleComponent();
		AController* NPCController = GetController<AController>();

		NPCCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		NPCMesh->bPauseAnims = true;
		NPCMesh->SetSimulatePhysics(true);

		// Add physics impuse to the corpse, if applicable.
		if (DamageCauser != nullptr) {
			float impulseMult = 1.0f;
			if (GameMode != nullptr) {
				impulseMult = GameMode->ProjectilePhysicsImpulseMultiplier;
			}
			FVector impulse = DamageCauser->GetVelocity();
			impulse.Normalize();
			impulse *= DamageAmount * 1000.0f * impulseMult;
			NPCMesh->AddImpulseAtLocation(impulse, DamageCauser->GetActorLocation());
		}

		// Destroy the NPC controller.
		if (NPCController != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("NPCController is valid, destroying."));
			NPCController->StopMovement();
			NPCController->UnPossess();
			NPCController->Destroy();
		}
		if (CurrentHealth > 0)
		{
			CurrentHealth = 0;
		}
		IsAlive = false;

		// Ragdoll cleanup time set up.
		float cleanupTime = 5.0f;
		if (GameMode != nullptr)
		{
			cleanupTime = GameMode->CorpseRagdollCleanupTime;
		}
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &ABaseNPC::DeathCleanup, cleanupTime, false);
	}
}

