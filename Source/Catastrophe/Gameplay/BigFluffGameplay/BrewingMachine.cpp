// Fill out your copyright notice in the Description page of Project Settings.


#include "BrewingMachine.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"

#include "Interactable/BaseClasses/InteractableComponent.h"
#include "BrewingMachineAnimInstance.h"

#include "DebugUtility/CatastropheDebug.h"

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
	InteractableComponent->OnInteractTickBegin.RemoveDynamic(this, &ABrewingMachine::OnInteractBegin);
	InteractableComponent->OnInteractTickBegin.AddDynamic(this, &ABrewingMachine::OnInteractBegin);
	InteractableComponent->OnInteractSuccess.RemoveDynamic(this, &ABrewingMachine::OnInteractSuccess);
	InteractableComponent->OnInteractSuccess.AddDynamic(this, &ABrewingMachine::OnInteractSuccess);
}

// Called when the game starts or when spawned
void ABrewingMachine::BeginPlay()
{
	Super::BeginPlay();
	
	BrewingMachineAnimInstance = Cast<UBrewingMachineAnimInstance>(BrewingMachineMesh->GetAnimInstance());
	if (!BrewingMachineAnimInstance)
		CatastropheDebug::OnScreenErrorMsg(TEXT("BrewingMachine: Invalid anim instance"));

}

// Called when the player interact with the brewing machine
void ABrewingMachine::OnInteractSuccess(class APlayerCharacter* _playerCharacter)
{

}

void ABrewingMachine::OnInteractBegin(class APlayerCharacter* _playerCharacter)
{

}

// Called every frame
void ABrewingMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

