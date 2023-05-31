// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.


#include "./BaseNPC.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/DamageType.h"

#include "../ProjectHoplomancerGameModeBase.h"

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
	LastAttackTime = -99.0f;
	AttackPrimaryCooldownTime = 0.5f;
	AttackPrimaryDamage = 10.0f;
	AttackPrimaryRange = 100.0f;  // One meter.
	AttackPhysicsImpulse = 800.0f;
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
		double physicsImpulse = DamageEvent.DamageTypeClass.GetDefaultObject()->DamageImpulse;
		HandleDeath(DamageAmount, EventInstigator, physicsImpulse, DamageCauser);
	}

	return res;
}


void ABaseNPC::PrimaryAttack()
{
	if (GetWorld()->GetTimeSeconds() >= LastAttackTime + AttackPrimaryCooldownTime)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "NPC is primary attacking!");
		LastAttackTime = GetWorld()->GetTimeSeconds();

		// Set what actors to seek out from its collision channel
		TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
		traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

		// Actors to ignore...
		TArray<AActor*> ignoredActors;
		// Ignore ourselves.
		ignoredActors.Init(this, 1);

		// Array for actors that are inside the radius of the sphere.
		TArray<AActor*> overlappedActors;

		// Total radius of the sphere:
		float r = AttackPrimaryRange;
		FVector location = GetActorLocation();

		UClass* seekClass = APlayerCharacter::StaticClass();
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), location, r, traceObjectTypes, seekClass, ignoredActors, overlappedActors);

		DrawDebugSphere(GetWorld(), GetActorLocation(), r, 12, FColor::Red, false, 3.0f);

		if (overlappedActors.Num() == 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Sphere trace didn't hit anything!");
		}

		for (AActor* a : overlappedActors)
		{
			FString n = FString::Printf(TEXT("NPC hit %s!"), *a->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, n);
			APlayerCharacter* p = Cast<APlayerCharacter>(a);
			if (p != nullptr)
			{
				TSubclassOf<UDamageType> damageType = UDamageType::StaticClass();
				p->TakeDamage(AttackPrimaryDamage, FDamageEvent(damageType), GetController(), this);
				FVector pos = GetActorLocation();
				pos.Z = p->GetActorLocation().Z - 10.0f;  // Impulse should always be angled just slightly upward...
				p->AddPhysicsImpulse(pos, AttackPhysicsImpulse);
			}
		}
	}
}


void ABaseNPC::DeathCleanup()
{
	Destroy();
}


void ABaseNPC::HandleDeath(float DamageAmount, AController* EventInstigator, double PhysicsImpulse, AActor* DamageCauser)
{
	if (IsAlive)
	{
		AProjectHoplomancerGameModeBase* GameMode = Cast<AProjectHoplomancerGameModeBase>(GetWorld()->GetAuthGameMode());

		if (EventInstigator != nullptr)
		{
			FString out = FString::Printf(TEXT("NPC death causer is %s!"), *EventInstigator->GetPawn()->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, out);
			ACharacter* inst = Cast<ACharacter>(EventInstigator->GetPawn());
			if (inst == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
			{
				// Only change the score if the event instigator for this NPC's death was the player.
				if (GameMode != nullptr)
				{
					GameMode->AddToPlayerScore(PlayerScorePointsValue);
				}
				else
				{
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

		// Add physics impulse to the corpse, if applicable.
		if (DamageCauser != nullptr)
		{
			float deathMult = 1.0f;
			if (GameMode != nullptr)
			{
				deathMult *= GameMode->NPCDeathPhysicsImpulseMultiplier;  // Make the killing hit have a stronger physics impulse.
			}

			FVector impulse = DamageCauser->GetVelocity();
			impulse.Normalize();
			impulse *= PhysicsImpulse * deathMult;
			FString out = FString::Printf(TEXT("NPC death impulse strength is %f!"), impulse.Length());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, out);
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

