#include "./BasePickupableItem.h"

#include "../PlayerCharacter/PlayerCharacter.h"


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
}


void ABasePickupableItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	bool success = HandlePickupItem(OtherActor);
	if (success)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Can pick up the item!"));
		Activate(OtherActor);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Couldn't pick up item!"));
	}

	Super::NotifyActorBeginOverlap(OtherActor);
}


bool ABasePickupableItem::HandlePickupItem(AActor* OtherActor)
{
	APlayerCharacter* p = Cast<APlayerCharacter>(OtherActor);
	if (p != nullptr && p->IsAlive)
	{
		return true;
	}
	return false;
}


void ABasePickupableItem::Activate(AActor* OtherActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Activating item."));
	SetActorEnableCollision(false);
	WorldMesh->SetVisibility(false);
	Destroy();
}
