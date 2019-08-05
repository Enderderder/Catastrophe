// Fill out your copyright notice in the Description page of Project Settings.

#include "TomatoBox.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

#include "Engine/CollisionProfile.h"

#include "Interactable/BaseClasses/InteractableComponent.h"
#include "Characters/PlayerCharacter/InventoryComponent.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"

#include "Characters/PlayerCharacter/TomatoSack.h"

ATomatoBox::ATomatoBox()
{
	PrimaryActorTick.bCanEverTick = false;

	TomatoBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TomatoBoxMesh"));
	TomatoBoxMesh->SetGenerateOverlapEvents(false);
	TomatoBoxMesh->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	RootComponent = TomatoBoxMesh;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetGenerateOverlapEvents(true);
	TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));
	TriggerVolume->SetupAttachment(RootComponent);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->bOneTimeUse = false;
	InteractableComponent->RegisterTriggerVolume(TriggerVolume);
	InteractableComponent->OnInteract.AddDynamic(this, &ATomatoBox::PickUpTomato);
}

void ATomatoBox::BeginPlay()
{
	Super::BeginPlay();

}

void ATomatoBox::PickUpTomato(APlayerCharacter* _playerCharacter)
{
	Receive_PickUpTomato();

	UInventoryComponent* inventoryComp = _playerCharacter->GetInventoryComponent();

	// Restore the tomato accordingly
	if (bRestoreAllTomatoOneTime)
	{
		//_playerCharacter->RestoreAllTomatos();
		//playInventoryComponent->
	}
	else
	{
		inventoryComp->PickupItem(ATomatoSack::StaticClass());
	}
}
