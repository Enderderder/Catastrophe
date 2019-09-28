// Fill out your copyright notice in the Description page of Project Settings.


#include "BrewingMachine.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"

#include "Interactable/BaseClasses/InteractableComponent.h"

// Sets default values
ABrewingMachine::ABrewingMachine()
{
 	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	BrewingMachineMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BrewingMachineMesh"));
	BrewingMachineMesh->SetGenerateOverlapEvents(false);
	BrewingMachineMesh->SetupAttachment(RootComponent);

	MeshCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("MeshCollider"));
	MeshCollider->SetGenerateOverlapEvents(false);
	MeshCollider->SetCollisionProfileName(TEXT("BlockAll"));
	MeshCollider->SetupAttachment(BrewingMachineMesh);

	InteractionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionTrigger"));
	InteractionTrigger->SetCollisionProfileName(TEXT("Trigger"));
	InteractionTrigger->SetupAttachment(BrewingMachineMesh);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->RegisterTriggerVolume(InteractionTrigger);
	InteractableComponent->OnInteract.RemoveDynamic(this, &ABrewingMachine::OnInteract);
	InteractableComponent->OnInteract.AddDynamic(this, &ABrewingMachine::OnInteract);
}

// Called when the game starts or when spawned
void ABrewingMachine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called when the player interact with the brewing machine
void ABrewingMachine::OnInteract(class APlayerCharacter* _playerCharacter)
{

}

// Called every frame
void ABrewingMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

