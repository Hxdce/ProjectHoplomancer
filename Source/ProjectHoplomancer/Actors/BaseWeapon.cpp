// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actors/BaseWeapon.h"

#include "../PlayerCharacter/PlayerCharacter.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up collider.
	PickupCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	PickupCollider->InitSphereRadius(100.0f);
	PickupCollider->BodyInstance.SetCollisionProfileName(TEXT("Trigger"));
	SetRootComponent(PickupCollider);

	// Set up third person mesh.
	ThirdPersonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ThirdPersonMesh"));
	ThirdPersonMesh->SetupAttachment(GetRootComponent());

	// Default value for PrintName.
	PrintName = "Base Weapon";
	// IMPORTANT: If a derived child class prints the name "Base Weapon", it needs to have its Print Name value overriden
	// in its respective blueprint!!!
}


// Called when the game starts or when spawned.
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ABaseWeapon BeginPlay!"));
}


// Called every frame.
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Handle actor overlapping this.
void ABaseWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	APlayerCharacter* p = Cast<APlayerCharacter>(OtherActor);
	if (p != nullptr)
	{
		bool success = p->TakeWeapon(this);
		if (!success)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Couldn't pick up weapon!"));
		}
	}

	Super::NotifyActorBeginOverlap(OtherActor);
}


void ABaseWeapon::PrimaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation)
{
	ReceiveWeaponFire(Parent, MuzzleLocation);
	OnWeaponFire.Broadcast(Parent, MuzzleLocation);
}


void ABaseWeapon::SecondaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation)
{
}


float ABaseWeapon::GetNextFireTime()
{
	return NextFireTime;
}


void ABaseWeapon::SetNextFireTime(double Time)
{
	NextFireTime = Time;
}


void ABaseWeapon::SetThirdPersonMeshVisibility(bool Vis)
{
	ThirdPersonMesh->SetVisibility(Vis);
}


bool ABaseWeapon::GetThirdPersonMeshVisibility()
{
	return ThirdPersonMesh->IsVisible();
}


void ABaseWeapon::ReloadWeapon(bool EmptyReload)
{
	if (ReservoirCurrRoundCount < ReservoirMax)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Reloading Weapon!"));
		ReservoirCurrRoundCount = ReservoirMax;
	}
}

