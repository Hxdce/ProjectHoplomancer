// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.


#include "./ShotgunPumpAction.h"

#include "../../PlayerCharacter/PlayerCharacter.h"


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
	ReloadDuration = 0.5f;
	ReloadFireDelayTime = 1.1f;  // Since the firing and pumping are currently combined into one, this needs to pretty long!
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

		// Create and fire off the shotgun pellets.
		for (int i = 0; i < ProjectileCount; i++)
		{
			// Set the projectile's initial rotation.
			FVector FiringDirection = MuzzleRotation.Vector();
			// Randomize projectile directions for shotgun cone of fire.
			AddSpreadToProjectile(&FiringDirection);
			ABaseProjectile* Projectile = World->SpawnActor<ABaseProjectile>(WeaponProjectile, MuzzleLocation, FiringDirection.Rotation(), SP);
			if (Projectile)
			{
				fired = true;
				// Set up projectile properties here? (Might be better to make this its own separate function, or distinct ammo type classes!)
				Projectile->ProjectileFirer = SP.Owner->GetInstigatorController();
				Projectile->MovementComponent->InitialSpeed = ProjectileVelocity;
				Projectile->MovementComponent->MaxSpeed = ProjectileVelocity;
				Projectile->DamageValue = DamagePrimary;

				// Fire the projectile.
				Projectile->FireInDirection(FiringDirection);

			}
		}
		
		/*
		FVector traceStart = MuzzleLocation;
		FVector traceEnd = MuzzleLocation + MuzzleRotation.Vector() * 100000.0f;

		//DrawDebugLine(GetWorld(), traceStart, traceEnd, FColor::Red, false, 5.0f, 0, 2.0f);
		float coneW = FMath::DegreesToRadians(WeaponSpread/2.0);
		float coneH = coneW;
		DrawDebugCone(GetWorld(), traceStart, traceEnd, 100000.0f, coneW, coneH, 32, FColor::Red, false, 5.0f);
		*/
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
void AShotgunPumpAction::SecondaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation)
{

}


void AShotgunPumpAction::ReloadStart()
{
	if (TimeCanReload > GetWorld()->GetTimeSeconds())
	{
		QueuedReload = true;
		return;
	}
	else
	{
		QueuedReload = false;
	}

	int roundsAvailable = TotalAmmoCount - ReservoirCurrRoundCount;  // How many rounds are actually available.
	if (ReservoirCurrRoundCount < ReservoirMax && roundsAvailable > 0 && !IsReloading)
	{
		IsReloading = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Reloading Weapon!"));
		// Set timed function for inserting a shell.
		FTimerManager* TimerMgr = &GetWorld()->GetTimerManager();
		FTimerHandle handleReload;
		TimerMgr->SetTimer(handleReload, this, &AShotgunPumpAction::ReloadInsertShell, ReloadDuration/2, false);
	}
}


void AShotgunPumpAction::ReloadFinish()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Finished reloading!"));
	IsReloading = false;
}


void AShotgunPumpAction::ReloadInsertShell()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(Wielder);
	if (player == nullptr || !player->IsAlive)
	{
		IsReloading = false;
		return;
	}

	int roundsAvailable = TotalAmmoCount - ReservoirCurrRoundCount;  // How many rounds are actually available.
	bool continueReload = false;

	if (ReservoirCurrRoundCount < ReservoirMax && roundsAvailable > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Inserting Shell!"));
		ReservoirCurrRoundCount += 1;
		if (SoundInsertShell)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundInsertShell , GetActorLocation(), FRotator::ZeroRotator);
		}
		continueReload = ReservoirCurrRoundCount < ReservoirMax && roundsAvailable > 0;
	}

	FTimerManager* TimerMgr = &GetWorld()->GetTimerManager();
	if (continueReload)
	{
		// Set timed function for inserting the next shell.
		FTimerHandle handleReload;
		TimerMgr->SetTimer(handleReload, this, &AShotgunPumpAction::ReloadInsertShell, ReloadDuration, false);
	}
	else
	{
		// Set timed function for finishing the reload.
		FTimerHandle finishReload;
		TimerMgr->SetTimer(finishReload, this, &AShotgunPumpAction::ReloadFinish, 0.15f, false);
	}
}
