// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.


#include "./BaseProjectile.h"

#include "Engine/DamageEvents.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "../ProjectHoplomancerGameModeBase.h"
#include "./BaseNPC.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	Collider->InitSphereRadius(4.0f);
	Collider->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	Collider->SetEnableGravity(false);

	RootComponent = Collider;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	MovementComponent->UpdatedComponent = Collider;
	MovementComponent->InitialSpeed = 3000.0f;
	MovementComponent->MaxSpeed = 3000.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->ProjectileGravityScale = 0.0f;

	DamageValue = 1.0f;
	InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	Collider->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnOverlapBegin);
}


// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Invoked when hitting something.
void ABaseProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	bool isNotNull = OtherActor != nullptr && OtherComponent != nullptr;

	
	if (isNotNull)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, OtherComponent->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, OtherActor->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Invoking OnHit");
	}
	

	if (OtherActor != this && OtherActor != this->GetOwner())
	{
		// Mass in grains / grains per gram * grams per kilogram.
		double projectileMassKG = ProjectileMass / (15.4323584 * 1000.0);
		// 100 Unreal units = 1 meter so divide this by 100 to get meters/sec.
		double projectileVelocity = MovementComponent->Velocity.Length() / 100.0;


		// Set up the final impulse variable.
		// Bullets don't impart much force in real life so we need this pretty high for it to be noticeabe!
		double impulseMult = 1000.0f;
		AProjectHoplomancerGameModeBase* GameMode = Cast<AProjectHoplomancerGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode != nullptr)
		{
			impulseMult = GameMode->ProjectilePhysicsImpulseMultiplier;
		}
		FVector impulse = MovementComponent->Velocity;
		impulse.Normalize();
		// Impulse is represented in newton-seconds.
		impulse *= projectileMassKG * projectileVelocity * impulseMult;

		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Impulse set up!");

		if (OtherComponent->IsSimulatingPhysics())
		{
			FString out = FString::Printf(TEXT("Physics object impulse strength is %f!"), impulse.Length());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, out);
			OtherComponent->AddImpulseAtLocation(impulse, Hit.ImpactPoint);
		}
		else
		{
			ABaseNPC* npc = Cast<ABaseNPC>(OtherActor);
			if (npc != nullptr)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Hitting an NPC");
				float damage = DamageValue;
				if (Hit.Component != nullptr)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Hit.Component.Get()->GetName());
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Hit.BoneName.ToString());
					if (Hit.BoneName.ToString() == "head")
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Head");
						damage *= 3.0;
					}
				}


				TSubclassOf<UDamageType> damageType = UDamageType::StaticClass();
				damageType.GetDefaultObject()->DamageImpulse = impulse.Length();
				npc->TakeDamage(damage, FDamageEvent(damageType), ProjectileFirer, this);
			}
		}

	}
	Destroy();
}


// Invoked when overlapping something.
void ABaseProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* p = Cast<APlayerCharacter>(OtherActor);
	
	if (p == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, OtherComponent->GetName());
	}
	else
	{
		return;
	}
	/*
	if (OtherActor != this && OtherActor != this->GetOwner())
	{
		ABaseNPC* npc = Cast<ABaseNPC>(OtherActor);
		if (npc != nullptr)
		{
			//npc->InflictDamage(DamageValue);
			// UDamageType::StaticClass();
			TSubclassOf<UDamageType> damageType = UDamageType::StaticClass();
			npc->TakeDamage(DamageValue, FDamageEvent(damageType), ProjectileFirer, this);
		}
		Destroy();
	}
	*/
}


// Fire the projectile in a given direction.
void ABaseProjectile::FireInDirection(const FVector& ShootDirection)
{
	MovementComponent->Velocity = ShootDirection * MovementComponent->InitialSpeed;
}

