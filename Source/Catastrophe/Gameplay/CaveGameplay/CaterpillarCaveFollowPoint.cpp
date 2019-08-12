// Fill out your copyright notice in the Description page of Project Settings.


#include "CaterpillarCaveFollowPoint.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
ACaterpillarCaveFollowPoint::ACaterpillarCaveFollowPoint()
{
	// This actor never needs to be ticked
	PrimaryActorTick.bCanEverTick = false;

	EditorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EditorMesh"));
	EditorMesh->bIsEditorOnly = true;
	EditorMesh->bTickInEditor = false;
	EditorMesh->SetGenerateOverlapEvents(false);
	EditorMesh->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = EditorMesh;
}

// Called when the game starts or when spawned
void ACaterpillarCaveFollowPoint::BeginPlay()
{
	Super::BeginPlay();
	
}