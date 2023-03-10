// Fill out your copyright notice in the Description page of Project Settings.


#include "./BaseProjectile.h"
#include "../PlayerCharacter/PlayerCharacter.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	Collider->InitSphereRadius(4.0f);
	Collider->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

	RootComponent = Collider;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	MovementComponent->UpdatedComponent = Collider;
	MovementComponent->InitialSpeed = 3000.0f;
	MovementComponent->MaxSpeed = 3000.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	Collider->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
}


// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Invoked when hitting something.
void ABaseProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, OtherComponent->GetName());
	if (OtherActor != this && OtherActor != this->GetOwner())
	{
		if (OtherComponent->IsSimulatingPhysics())
		{
			// Make impulse velocity independent for now...
			FVector impulse = MovementComponent->Velocity;
			impulse.Normalize();
			impulse *= 20000.0f;
			OtherComponent->AddImpulseAtLocation(impulse, Hit.ImpactPoint);
		}
		Destroy();
	}
}


// Fire the projectile in a given direction.
void ABaseProjectile::FireInDirection(const FVector& ShootDirection)
{
	MovementComponent->Velocity = ShootDirection * MovementComponent->InitialSpeed;
}

