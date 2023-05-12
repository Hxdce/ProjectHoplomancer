#include "./BasePickupableItem.h"


// Sets default values
ABasePickupableItem::ABasePickupableItem()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up collider.
	PickupCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	PickupCollider->InitSphereRadius(100.0f);
	PickupCollider->BodyInstance.SetCollisionProfileName(TEXT("Trigger"));
	SetRootComponent(PickupCollider);

	// Set up world mesh.
	WorldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ThirdPersonMesh"));
	WorldMesh->SetupAttachment(GetRootComponent());

	OverlappingPlayerPtr = nullptr;
	PlayerIsOverlapping = false;
	NextPlayerOverlapRecheckTime = 0.0;
}


// Called when the game starts or when spawned
void ABasePickupableItem::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ABasePickupableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// This checks each tick if an overlapping player meets the pickup criteria.
	// This means if something happens while the player is standing over an item, we can recheck
	// if the pickup criteria is now met. E.g. taking damage at 100HP while standing over a healing item.
	// We ONLY want to do this if the player is indeed overlapping for performance reasons.
	if (PlayerIsOverlapping && GetWorld()->GetTimeSeconds() > NextPlayerOverlapRecheckTime)
	{
		APlayerCharacter* p = OverlappingPlayerPtr;
		if (p != nullptr && p->IsAlive)
		{
			bool success = HandlePickupItem(p);
			if (success)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Can pick up the item!"));
				Activate(p);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Couldn't pick up item!"));
			}
		}
		NextPlayerOverlapRecheckTime = GetWorld()->GetTimeSeconds() + 1.0 / 20.0;  // Every 50 milliseconds.
	}
}


void ABasePickupableItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	APlayerCharacter* p = Cast<APlayerCharacter>(OtherActor);
	if (p != nullptr && p->IsAlive) // Ignore anything that isn't the player, and the player has to be alive.
	{
		PlayerIsOverlapping = true;
		OverlappingPlayerPtr = p;
		bool success = HandlePickupItem(p);
		if (success)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Can pick up the item!"));
			Activate(p);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Couldn't pick up item!"));
		}
	}
	Super::NotifyActorBeginOverlap(OtherActor);
}


void ABasePickupableItem::NotifyActorEndOverlap(AActor* OtherActor)
{
	PlayerIsOverlapping = false;
	OverlappingPlayerPtr = nullptr;
	Super::NotifyActorEndOverlap(OtherActor);
}


bool ABasePickupableItem::HandlePickupItem(APlayerCharacter* PlayerActor)
{
	APlayerCharacter* p = PlayerActor;
	if (p != nullptr && p->IsAlive)
	{
		return true;
	}
	return false;
}


void ABasePickupableItem::Activate(APlayerCharacter* PlayerActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Activating item."));
	SetActorEnableCollision(false);
	WorldMesh->SetVisibility(false);
	Destroy();
}
