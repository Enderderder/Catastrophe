// Fill out your copyright notice in the Description page of Project Settings.


#include "VentilationShortcut.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Controller.h"

#include "Interactable/BaseClasses/InteractableComponent.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"

// Sets default values
AVentilationShortcut::AVentilationShortcut()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCanEverAffectNavigation(false);
	TriggerBox->SetGenerateOverlapEvents(true);
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetupAttachment(RootComponent);

	PopVentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PopVentMesh"));
	PopVentMesh->SetCanEverAffectNavigation(false);
	PopVentMesh->SetGenerateOverlapEvents(false);
	PopVentMesh->SetCollisionProfileName(TEXT("BlockAll"));
	PopVentMesh->SetupAttachment(RootComponent);

	TeleportTransformComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TeleportTransformComponent"));
	TeleportTransformComponent->SetupAttachment(RootComponent);

	TeleportArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("TeleportArrow"));
	TeleportArrow->SetupAttachment(TeleportTransformComponent);

	InteractComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractComponent"));
	InteractComponent->RegisterTriggerVolume(TriggerBox);
	InteractComponent->OnPlayerEnterInteractRange.RemoveDynamic(this, &AVentilationShortcut::OnPlayerEnterInteractRange);
	InteractComponent->OnPlayerEnterInteractRange.AddDynamic(this, &AVentilationShortcut::OnPlayerEnterInteractRange);
	InteractComponent->OnInteractSuccess.RemoveDynamic(this, &AVentilationShortcut::OnInteractSuccess);
	InteractComponent->OnInteractSuccess.AddDynamic(this, &AVentilationShortcut::OnInteractSuccess);
}

void AVentilationShortcut::OnPlayerEnterInteractRange(class APlayerCharacter* _playerCharacter)
{
	Receive_ShowHint(_playerCharacter, HintText);
}

void AVentilationShortcut::OnInteractSuccess(class APlayerCharacter* _playerCharacter)
{
	if (bLocked)
	{
		Receive_ShowHint(_playerCharacter, HintTextWhenLocked);
	}
	else
	{
		FVector teleportLocation = TeleportTransformComponent->GetComponentLocation();
		FRotator teleportRotation = TeleportTransformComponent->GetComponentRotation();
		_playerCharacter->SetActorLocationAndRotation(teleportLocation, teleportRotation);
		_playerCharacter->GetController()->SetControlRotation(teleportRotation);
	}
}

// Called when the game starts or when spawned
void AVentilationShortcut::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVentilationShortcut::PopVentCap()
{
	bLocked = false;

	PopVentMesh->SetSimulatePhysics(true);

	FVector forceDirection =
		PopVentMesh->GetForwardVector().RotateAngleAxis(VentPopUpDegree, FVector(0.0f, 1.0f, 0.0f));
	FVector forceVector = forceDirection * VentPopForce * (PopVentMesh->GetMass());
	PopVentMesh->AddImpulseAtLocation(forceVector, PopVentMesh->GetComponentLocation() + FVector(0.0f, 0.0f, 10.0f));

	Receive_PopVentCap();
}
