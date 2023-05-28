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
	DamagePrimary = 6.0f;
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

}


// Secondary attack!
void AShotgunPumpAction::SecondaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation)
{

}
