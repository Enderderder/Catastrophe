// Fill out your copyright notice in the Description page of Project Settings.


#include "BrewingMachine.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"

#include "Interactable/BaseClasses/InteractableComponent.h"
#include "BrewingMachineAnimInstance.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"
#include "Components/BackPackComponent.h"
#include "Gameplay/Items/ItemBase.h"
#include "Gameplay/Items/ItemStack.h"

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
	UBackPackComponent* playerBackpack = _playerCharacter->GetBackPack();
	if (playerBackpack)
	{
		if (CheckRequiredItems(RequestCombineItems, playerBackpack))
		{
			// If player do have all these stuff
			for (FRequestItemInfo request : RequestCombineItems)
			{
				playerBackpack->RemoveItem(request.ItemClass, request.Amount);
			}

			/// Do whatever the machine things
		}
		else
		{
			/// Do whatever the machine things if player dont have enough material
		}
	}
}

void ABrewingMachine::OnInteractBegin(class APlayerCharacter* _playerCharacter)
{
	
}

bool ABrewingMachine::CheckRequiredItems(const TArray<FRequestItemInfo> _requestItems, class UBackPackComponent* _backpack)
{
	if (!IsValid(_backpack)) return false;

	for (FRequestItemInfo request : _requestItems)
	{
		const TArray<UItemStack*> stacks = _backpack->FindAllItemStack(request.ItemClass);
		if (_backpack->GetCombinedStackSize(stacks) < request.Amount)
			return false;
	}

	// Player do have sufficient items
	return true;
}

// Called every frame
void ABrewingMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

