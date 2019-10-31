// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableItem.h"

#include "Components/BoxComponent.h"

#include "Interactable/BaseClasses/InteractableComponent.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"
#include "Components/BackPackComponent.h"

#include "DebugUtility/CatastropheDebug.h"


// Default constructor
ACollectableItem::ACollectableItem()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRoot;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));
	TriggerVolume->SetCanEverAffectNavigation(false);
	TriggerVolume->SetCastShadow(false);
	TriggerVolume->SetupAttachment(RootComponent);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->bAutoInteract = false;
	InteractableComponent->RegisterTriggerVolume(TriggerVolume);
	InteractableComponent->OnInteractSuccess.RemoveDynamic(this, &ACollectableItem::OnInteractSuccess);
	InteractableComponent->OnInteractSuccess.AddDynamic(this, &ACollectableItem::OnInteractSuccess);
	InteractableComponent->InteractionDescriptionText = TEXT("Collect");
}

void ACollectableItem::OnInteractSuccess(class APlayerCharacter* _playerCharacter)
{
	UBackPackComponent* backpack = _playerCharacter->GetBackPack();
	if (!backpack) return;
	bool bInsertResult = backpack->InsertItem(this);
	if (!bInsertResult)
	{
		CatastropheDebug::OnScreenDebugMsg(-1, 10.0f, FColor::Yellow, TEXT("Cannot collect item, maybe backpack full?"));
		return;
	}
	
	// If successed, then no longer can interact
	InteractableComponent->bCanInteract = false;

	// Plays after effect and maybe delete actor
	AfterCollectedEffect(_playerCharacter);
	if (bDestroyImmediateAfterCollect) this->Destroy();
}

void ACollectableItem::AfterCollectedEffect_Implementation(APlayerCharacter* _playerCharacter)
{
	_playerCharacter->Receive_PlayGrabAnim();
}
