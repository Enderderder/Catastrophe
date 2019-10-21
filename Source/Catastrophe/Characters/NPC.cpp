// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Interactable/BaseClasses/InteractableComponent.h"
#include "QuestSystem/QuestObjectiveComponent.h"
#include "PlayerCharacter/PlayerCharacter.h"
#include "DialogueSystem/DialogueSystemComponent.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionProfileName("Trigger");
	TriggerBox->SetupAttachment(root);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->InteractionDescriptionText = TEXT("Talk");
	InteractableComponent->RegisterTriggerVolume(TriggerBox);
	InteractableComponent->OnInteractSuccess.AddDynamic(this, &ANPC::Interact);

	DialogueSystemComponent = CreateDefaultSubobject<UDialogueSystemComponent>(TEXT("DialogueSystemComponent"));
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC::Interact(class APlayerCharacter* _playerCharacter)
{
	Receive_Interact();
}
