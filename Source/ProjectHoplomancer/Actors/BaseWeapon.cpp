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

	NextFireTime = 0.0;
	IsReloading = false;
	Wielder = nullptr;

	// Default values for weapon sats.
	DamagePrimary = 10.0;
	DamageSecondary = 10.0;
	Firerate = 0.5;
	ProjectileVelocity = 5000.0;
	ReloadDuration = 1.0;
	RecoilPitchMin = 0.0;
	RecoilPitchMax = 0.0;
	RecoilYawMin = 0.0;
	RecoilYawMax = 0.0;
	RecoilSnappiness = 0.0;
	ReservoirMax = 8;
	ReservoirCurrRoundCount = ReservoirMax;
	TotalAmmoCount = ReservoirMax * 3;
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



bool ABaseWeapon::GetThirdPersonMeshVisibility()
{
	return ThirdPersonMesh->IsVisible();
}


void ABaseWeapon::SetThirdPersonMeshVisibility(bool Vis)
{
	ThirdPersonMesh->SetVisibility(Vis);
}


ACharacter* ABaseWeapon::GetWielder()
{
	return Wielder;
}


void ABaseWeapon::SetWielder(ACharacter* NewWielder)
{
	Wielder = NewWielder;
}


void ABaseWeapon::ReloadWeapon(bool EmptyReload)
{
	int roundsAvailable = TotalAmmoCount - ReservoirCurrRoundCount;  // How many rounds are actually available.
	if (ReservoirCurrRoundCount < ReservoirMax && roundsAvailable > 0 && !IsReloading)
	{
		IsReloading = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Reloading Weapon!"));

		// Set timed function for finishing the reload.
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &ABaseWeapon::ReloadFinish, ReloadDuration, false);
	}
	else if (ReservoirCurrRoundCount == ReservoirMax)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Don't need to reload!"));
	}
	else if (roundsAvailable <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("No extra ammo!"));
	}
}


void ABaseWeapon::ReloadFinish()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(Wielder);
	if (player != nullptr && player->IsAlive)
	{
		// Might be wise to make the weapon binded to the OnPlayerDeath delegate soon,
		// then a function for cancelling the reload can just be invoked through that.

		int roundsAvailable = TotalAmmoCount - ReservoirCurrRoundCount;  // How many rounds are actually available.
		int roundsNeeded = ReservoirMax - ReservoirCurrRoundCount;  // How many rounds are needed to fully reload.
		// Add the rounds needed if we got enough to fully fill the gun up, or just add the rest we have left.
		ReservoirCurrRoundCount += FMath::Min(roundsAvailable, roundsNeeded);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Finished reloading!"));
	}
	IsReloading = false;
}


void ABaseWeapon::ApplyRecoil()
{
	if (Wielder == nullptr)
	{
		return;
	}
	APlayerCharacter* player = Cast<APlayerCharacter>(Wielder);
	if (player != nullptr)
	{
		double pitch = FMath::RandRange(RecoilPitchMin, RecoilPitchMax);
		double yaw = FMath::RandRange(RecoilYawMin, RecoilYawMax);
		double roll = -yaw / 2.0;
		player->CameraApplyRecoil(FRotator(pitch, yaw, roll), RecoilSnappiness);
	}
}


int ABaseWeapon::GetCurrentReservoirRoundCount()
{
	return ReservoirCurrRoundCount;
}


void ABaseWeapon::SetCurrentReservoirRoundCount(int count)
{
	ReservoirCurrRoundCount = count;
}


int ABaseWeapon::GetTotalAmmoCount()
{
	return TotalAmmoCount;
}


void ABaseWeapon::GiveAmmo(int AmmoAmount)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Adding AmmoAmount's value to %s"), *PrintName));
	TotalAmmoCount = FMath::Min(TotalAmmoCount + AmmoAmount, AmmoMax);
}
