// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.


#include "./Pistol10mm.h"


// Sets default values.
APistol10mm::APistol10mm()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NextFireTime = -99.0;
	TimeCanReload = -99.0;

	// Default values for weapon stats:
	PrintName = "10mm Pistol";
	DamagePrimary = 20.0f;
	WeaponAccuracy = 0.5;
	RecoilSpreadMax = 3.0;
	RecoilSpreadTimeMax = 0.75;
	RecoilSpreadTimePerShot = 0.25;
	Firerate = 0.33333f;
	ProjectileVelocity = 10000.0f;
	RecoilPitchMin = 2.0f;
	RecoilPitchMax = 2.5f;
	RecoilYawMin = -0.5f;
	RecoilYawMax = 0.5f;
	RecoilSnappiness = 0.5f;
	ReservoirMax = 12;
	ReservoirCurrRoundCount = ReservoirMax;
	TotalAmmoCount = ReservoirMax * 3;
	AmmoType = AMMO_10MM;
	AmmoMax = 132;
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
void APistol10mm::PrimaryAttack(AActor* Parent, FVector MuzzleLocation, FVector MuzzleDirection)
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
			// Randomize projectile direction for weapon's baseline cone of fire.
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


// Secondary attack!
void APistol10mm::SecondaryAttack(AActor* Parent, FVector MuzzleLocation, FVector MuzzleDirection)
{
}