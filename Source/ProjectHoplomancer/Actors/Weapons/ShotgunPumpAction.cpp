// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.


#include "./ShotgunPumpAction.h"


// Sets default values.
AShotgunPumpAction::AShotgunPumpAction()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NextFireTime = -99.0f;

	// Default values for weapon stats:
	PrintName = "Pump-Action Shotgun";
	DamagePrimary = 7.0f;
	ProjectileCount = 8;
	Firerate = 1.5f;
	ProjectileVelocity = 10000.0f;
	RecoilPitchMin = 4.0f;
	RecoilPitchMax = 5.0f;
	RecoilYawMin = -1.5f;
	RecoilYawMax = 1.5f;
	RecoilSnappiness = 0.25f;
	ReservoirMax = 8;
	ReservoirCurrRoundCount = ReservoirMax;
	TotalAmmoCount = ReservoirMax * 3;
	AmmoType = AMMO_12GAUGE;
	AmmoMax = 72;
}


// Called when the game starts or when spawned.
void AShotgunPumpAction::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame.
void AShotgunPumpAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Primary attack!
void AShotgunPumpAction::PrimaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation)
{
	UWorld* World = GetWorld();
	// Basic check for valid world + can't fire faster than weapon firerate + can't fire while reloading.
	if (!Parent || !World || World->GetTimeSeconds() < NextFireTime || IsReloading)
	{
		return;
	}
	if (ReservoirCurrRoundCount <= 0)
	{
		// Should dry fire here, then reload if we have ammo.
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Weapon is empty! Click!"));
		NextFireTime = GetWorld()->GetTimeSeconds() + 0.5f;
		return;
	}

	if (WeaponProjectile)
	{
		FActorSpawnParameters SP;
		SP.Owner = Parent;
		if (SP.Owner != nullptr)
		{
			SP.Instigator = SP.Owner->GetInstigator();
		}

		bool fired = false;

		// Create and fire off the shotgun pellets.
		for (int i = 0; i < ProjectileCount; i++)
		{
			ABaseProjectile* Projectile = World->SpawnActor<ABaseProjectile>(WeaponProjectile, MuzzleLocation, MuzzleRotation, SP);
			if (Projectile)
			{
				fired = true;
				// Set up projectile properties here? (Might be better to make this its own separate function, or distinct ammo type classes!)
				Projectile->ProjectileFirer = SP.Owner->GetInstigatorController();
				Projectile->MovementComponent->InitialSpeed = ProjectileVelocity;
				Projectile->MovementComponent->MaxSpeed = ProjectileVelocity;
				Projectile->DamageValue = DamagePrimary;

				// Set the projectile's initial rotation.
				FRotator CurrentLaunchDirection = MuzzleRotation;

				// Randomize projectile directions for shotgun spread.
				// The projectile spread value from the player's POV represents the diameter of a circle in degrees.
				// Create a random polar coordinate within half of the max spread (that circle's radius), then
				// convert it to cartesian coordinates for the random pitch and yaw change.
				double r = FMath::RandRange(0.0, 3.0);
				double theta = FMath::RandRange(0.0, 360.0);
				double x = r * FMath::Cos(theta);
				double y = r * FMath::Sin(theta);
				CurrentLaunchDirection.Pitch += x;
				CurrentLaunchDirection.Yaw += y;

				// Randomize projectile directions for shotgun spread.
				Projectile->FireInDirection(CurrentLaunchDirection.Vector());

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
		}

		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Bang!"));
		NextFireTime = GetWorld()->GetTimeSeconds() + Firerate;
		ReservoirCurrRoundCount--;
		TotalAmmoCount--;

		if (SoundPrimaryAttack)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundPrimaryAttack, GetActorLocation(), FRotator::ZeroRotator);
		}
	}
}


// Secondary attack!
void AShotgunPumpAction::SecondaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation)
{

}
