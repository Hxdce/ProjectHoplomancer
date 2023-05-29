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
	WeaponSpread = 5.0;
	Firerate = 1.15f;
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
	// Basic check for valid wielder + valid world + can't fire faster than weapon firerate + can't fire while reloading.
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
				FRotator FiringDirection = MuzzleRotation;
				// Randomize projectile directions for shotgun cone of fire.
				AddSpreadToProjectile(&FiringDirection);
				// Fire the projectile.
				Projectile->FireInDirection(FiringDirection.Vector());

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
		}

		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Bang!"));
		NextFireTime = GetWorld()->GetTimeSeconds() + Firerate;
		ReservoirCurrRoundCount--;
		TotalAmmoCount--;
	}
}


// Secondary attack!
void AShotgunPumpAction::SecondaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation)
{

}
