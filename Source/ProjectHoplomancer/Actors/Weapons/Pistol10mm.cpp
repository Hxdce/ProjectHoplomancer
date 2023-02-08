// Fill out your copyright notice in the Description page of Project Settings.


#include "../Weapons/Pistol10mm.h"


// Sets default values.
APistol10mm::APistol10mm()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NextFireTime = -99.0f;
	Firerate = 0.25f;
}

// Called when the game starts or when spawned.
void APistol10mm::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame.
void APistol10mm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Primary attack!
void APistol10mm::PrimaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation)
{
	UWorld* World = GetWorld();
	// Basic check for valid world + can't fire faster than weapon firerate.
	if (!Parent || !World || World->GetTimeSeconds() < NextFireTime)
	{
		return;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Invoking 10mm pistol primary attack!"));


	if (WeaponProjectile)
	{		
		FActorSpawnParameters SP;
		SP.Owner = Parent;
		if (SP.Owner != nullptr)
		{
			SP.Instigator = SP.Owner->GetInstigator();
		}


		ABaseProjectile* Projectile = World->SpawnActor<ABaseProjectile>(WeaponProjectile, MuzzleLocation, MuzzleRotation, SP);
		if (Projectile)
		{
			// Set up projectile properties here? (Might be better to make this its own separate function, or distinct ammo type classes!)
			Projectile->MovementComponent->InitialSpeed = 10000.0f;
			Projectile->MovementComponent->MaxSpeed = 10000.0f;

			// Set the projectile's initial trajectory.
			FVector LaunchDirection = MuzzleRotation.Vector();
			Projectile->FireInDirection(LaunchDirection);
		}

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Bang!"));
		NextFireTime = GetWorld()->GetTimeSeconds() + Firerate;
	}


}


// Secondary attack!
void APistol10mm::SecondaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation)
{
}