// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemCrate.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

#include "Engine/CollisionProfile.h"

#include "Interactable/BaseClasses/InteractableComponent.h"
#include "Characters/PlayerCharacter/InventoryComponent.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"

#include "Characters/PlayerCharacter/ItemSack.h"

// Sets default values
AItemCrate::AItemCrate()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetGenerateOverlapEvents(false);
	ItemMesh->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	RootComponent = ItemMesh;

	OutlineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OutlineMesh"));
	OutlineMesh->SetGenerateOverlapEvents(false);
	OutlineMesh->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	OutlineMesh->SetupAttachment(ItemMesh);

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetGenerateOverlapEvents(true);
	TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));
	TriggerVolume->SetupAttachment(RootComponent);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->bOneTimeUse = false;
	InteractableComponent->RegisterTriggerVolume(TriggerVolume);
}

// Called when the game starts or when spawned
void AItemCrate::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Move to constructor when able to (not working atm)
	InteractableComponent->OnInteract.AddDynamic(this, &AItemCrate::PickupItem);
}

void AItemCrate::PickupItem(class APlayerCharacter* _playerCharacter)
{
	Receive_PickupItem();

	UInventoryComponent* inventoryComp = _playerCharacter->GetInventoryComponent();

	// Adds item type to inventory
	if (ItemSackType)
	{
		inventoryComp->PickupItem(ItemSackType);
	}
}

