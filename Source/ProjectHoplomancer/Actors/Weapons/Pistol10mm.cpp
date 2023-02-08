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
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Invoking 10mm pistol primary attack!"));
	// Can't fire faster than weapon firerate.
	if (GetWorld()->GetTimeSeconds() < NextFireTime)
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Bang!"));
	NextFireTime = GetWorld()->GetTimeSeconds() + Firerate;
}


// Secondary attack!
void APistol10mm::SecondaryAttack(AActor* Parent, FVector MuzzleLocation, FRotator MuzzleRotation)
{
}