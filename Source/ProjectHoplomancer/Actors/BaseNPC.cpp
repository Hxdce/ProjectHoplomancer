// Fill out your copyright notice in the Description page of Project Settings.


#include "./BaseNPC.h"

// Sets default values
ABaseNPC::ABaseNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxHealth = 100;
	CurrentHealth = MaxHealth;
	IsAlive = true;
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
		Die();
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


void ABaseNPC::Die()
{
	if (IsAlive)
	{
		USkeletalMeshComponent* NPCMesh = GetMesh();
		UCapsuleComponent* NPCCapsuleComponent = GetCapsuleComponent();
		AController* NPCController = GetController<AController>();

		NPCCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		NPCMesh->bPauseAnims = true;
		NPCMesh->SetSimulatePhysics(true);
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

		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &ABaseNPC::DeathCleanup, 3.0f, false);
	}
}

