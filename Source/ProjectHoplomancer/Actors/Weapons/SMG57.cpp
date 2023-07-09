// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.


#include "./SMG57.h"


// Sets default values.
ASMG57::ASMG57()
{
	PrimaryActorTick.bCanEverTick = true;
	NextFireTime = -99.0;
	TimeCanReload = -99.0;

	PrintName = "5.7mm SMG";
	DamagePrimary = 4.0f;
	WeaponAccuracy = 1.5;
	RecoilSpreadMax = 2.5;
	RecoilSpreadTimeMax = 0.28;
	RecoilSpreadTimePerShot = 0.08;
	Firerate = 0.06666f;
	ProjectileVelocity = 10000.0f;
	RecoilPitchMin = 0.75f;
	RecoilPitchMax = 1.2f;
	RecoilYawMin = -0.75f;
	RecoilYawMax = 0.75f;
	RecoilSnappiness = 0.75f;
	ReservoirMax = 50;
	ReservoirCurrRoundCount = ReservoirMax;
	TotalAmmoCount = ReservoirMax * 3;
	AmmoType = AMMO_57SMG;
	AmmoMax = 550;
}


// Called when the game starts or when spawned.
void ASMG57::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame.
void ASMG57::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Primary attack!
void ASMG57::PrimaryAttack(AActor* Parent, FVector MuzzleLocation, FVector MuzzleDirection)
{
	UWorld* World = GetWorld();
	// Basic check for valid wielder + valid world + can't fire faster than weapon firerate + can't fire while reloading.
	if (!Parent || !World || World->GetTimeSeconds() < NextFireTime || IsReloading)
	{
		return;
	}
	if (ReservoirCurrRoundCount <= 0)
	{
		// No ammo in gun.
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Weapon is empty!"));
	}

	bool fired = false;

	if (WeaponProjectile && ReservoirCurrRoundCount > 0)
	{
		FActorSpawnParameters SP;
		SP.Owner = Parent;
		if (SP.Owner != nullptr)
		{
			SP.Instigator = SP.Owner->GetInstigator();
		}

		// Add any spread penalties to the muzzle direction.
		AddWeaponSpreadPenalties(&MuzzleDirection);

		ABaseProjectile* Projectile = World->SpawnActor<ABaseProjectile>(WeaponProjectile, MuzzleLocation, MuzzleDirection.Rotation(), SP);
		if (Projectile)
		{
			fired = true;
			// Set up projectile properties here? (Might be better to make this its own separate function, or distinct ammo type classes!)
			Projectile->ProjectileFirer = SP.Owner->GetInstigatorController();
			Projectile->MovementComponent->InitialSpeed = ProjectileVelocity;
			Projectile->MovementComponent->MaxSpeed = ProjectileVelocity;
			Projectile->DamageValue = DamagePrimary;
			// Set the projectile's initial trajectory.
			FVector FiringDirection = MuzzleDirection;
			// Randomize projectile direction for weapon's cone of fire.
			AddBaselineSpreadToProjectile(&FiringDirection);
			// Fire the projectile.
			Projectile->FireInDirection(FiringDirection);

			//FVector traceStart = MuzzleLocation;
			//FVector traceEnd = MuzzleLocation + MuzzleRotation.Vector() * 100000.0f;

			//DrawDebugLine(GetWorld(), traceStart, traceEnd, FColor::Red, false, 5.0f, 0, 2.0f);
		}
	}

	if (fired)
	{
		ApplyRecoil();
		ReceiveWeaponFire(Parent, MuzzleLocation);
		OnWeaponFire.Broadcast(Parent, MuzzleLocation);
		APawn* soundMaker = Cast<APawn>(Parent);
		if (soundMaker != nullptr)
		{
			MakeNoise(1.0, soundMaker, GetActorLocation(), 50000.0f);
		}
		if (SoundPrimaryAttack)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundPrimaryAttack, GetActorLocation(), FRotator::ZeroRotator);
		}

		TimeCanReload = GetWorld()->GetTimeSeconds() + ReloadFireDelayTime;
		NextFireTime = GetWorld()->GetTimeSeconds() + Firerate;
		ReservoirCurrRoundCount--;
		TotalAmmoCount--;
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundDryFire, GetActorLocation(), FRotator::ZeroRotator);
		TimeCanReload = GetWorld()->GetTimeSeconds() + 0.1;
		NextFireTime = GetWorld()->GetTimeSeconds() + 0.5;
	}
}