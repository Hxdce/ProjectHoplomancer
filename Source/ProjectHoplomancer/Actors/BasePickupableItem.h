#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"

// This include always comes last:
#include "BasePickupableItem.generated.h"

UCLASS()
class PROJECTHOPLOMANCER_API ABasePickupableItem : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABasePickupableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Mesh for model.
	UPROPERTY(EditAnywhere, Category=Mesh)
	UStaticMeshComponent* WorldMesh;

	// Collider for pickup detection.
	UPROPERTY(VisibleDefaultsOnly, Category=Collision)
	class USphereComponent* PickupCollider;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Overriden functions:

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Proprietary functions:

	// Function for checking whether the item meets its necessary criteria for being picked up.
	// E.g. for a healing item, this would check if the player is injured.
	virtual bool HandlePickupItem(AActor* OtherActor);

	// Function for what the item does when picked up.
	virtual void Activate(AActor* OtherActor);
};
