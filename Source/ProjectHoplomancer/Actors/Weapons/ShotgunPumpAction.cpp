// © Hxdce, 1994 - Present. Created in 2023. This work is licensed under a CC BY-NC-SA 4.0 license.


#include "./ShotgunPumpAction.h"
#include "Kismet/KismetMathLibrary.h"

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
	WeaponAccuracy = 5.0;
	RecoilSpreadMax = 5.0;
	RecoilSpreadTimeMax = 0.3;
	RecoilSpreadTimePerShot = 0.2;
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


void AShotgunPumpAction::AddBaselineSpreadToProjectile(FVector* FiringDirection)
{
	// With shotguns, we don't want to modify the weapon spread like we do with other weapons.
	// We always want to keep the spread of the pellets the same no matter what, and simplify modify their direction
	// by the total amount of spread MINUS the intrinsic pellet spread.
	(*FiringDirection) = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(*FiringDirection, WeaponAccuracy / 2);
}


// Primary attack!
void AShotgunPumpAction::PrimaryAttack(AActor* Parent, FVector MuzzleLocation, FVector MuzzleDirection)
{
	UWorld* World = GetWorld();
	// Basic check for valid wielder + valid world + can't fire faster than weapon firerate + can't fire while reloading.
	if (!Parent || !World || World->GetTimeSeconds() < NextFireTime || IsReloading)
	{
		if (IsReloading && ReservoirCurrRoundCount > 0)
		{
			QueuedFiring = true;
		}
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

		// Create and fire off the shotgun pellets.
		for (int i = 0; i < ProjectileCount; i++)
		{
			// Set the projectile's initial rotation.
			FVector FiringDirection = MuzzleDirection;
			// Randomize projectile directions for shotgun cone of fire.
			AddBaselineSpreadToProjectile(&FiringDirection);
			ABaseProjectile* Projectile = World->SpawnActor<ABaseProjectile>(WeaponProjectile, MuzzleLocation, FiringDirection.Rotation(), SP);
			if (Projectile)
			{
				fired = true;
				QueuedFiring = false;
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
		FVector traceEnd = MuzzleLocation + MuzzleDirection * 100000.0f;

		//DrawDebugLine(GetWorld(), traceStart, traceEnd, FColor::Red, false, 5.0f, 0, 2.0f);
		float coneW = FMath::DegreesToRadians(WeaponAccuracy/2.0);
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
void AShotgunPumpAction::SecondaryAttack(AActor* Parent, FVector MuzzleLocation, FVector MuzzleDirection)
{

}


void AShotgunPumpAction::ReloadStart()
{
	if (!IsEquipped)
	{
		return;
	}

	if (QueuedFiring)  // Just make this interrupt reloading for now, no firing.
	{
		QueuedReload = false;
		QueuedFiring = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Interrupting reloading!"));
		IsReloading = false;
		return;
	}

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
		WeaponSpreadRecoilPenaltyTime = RecoilSpreadTimeMax;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Reloading Weapon!"));
		// Set timed function for inserting a shell.
		FTimerManager* TimerMgr = &GetWorld()->GetTimerManager();
		FTimerHandle handleReload;
		TimerMgr->SetTimer(handleReload, this, &AShotgunPumpAction::ReloadInsertShell, ReloadDuration/2, false);
	}
}


void AShotgunPumpAction::ReloadFinish()
{
	if (!IsEquipped)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Finished reloading!"));
	IsReloading = false;
}


void AShotgunPumpAction::ReloadInsertShell()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(Wielder);
	if (player == nullptr || !player->IsAlive || !IsEquipped)
	{
		IsReloading = false;
		return;
	}

	if (QueuedFiring)  // Just make this interrupt reloading for now, no firing.
	{
		QueuedReload = false;
		QueuedFiring = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Interrupting reloading!"));
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
