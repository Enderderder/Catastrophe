// Fill out your copyright notice in the Description page of Project Settings.


#include "CaterpillarCaveFollowPoint.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
ACaterpillarCaveFollowPoint::ACaterpillarCaveFollowPoint()
{
	// This actor never needs to be ticked
	PrimaryActorTick.bCanEverTick = false;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRoot;

	EditorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EditorMesh"));
	EditorMesh->bIsEditorOnly = true;
	EditorMesh->bTickInEditor = false;
	EditorMesh->SetGenerateOverlapEvents(false);
	EditorMesh->SetCollisionProfileName(TEXT("NoCollision"));
	EditorMesh->SetupAttachment(DefaultRoot);
}

// Called when the game starts or when spawned
void ACaterpillarCaveFollowPoint::BeginPlay()
{
	Super::BeginPlay();
	
}