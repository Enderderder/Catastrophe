// Fill out your copyright notice in the Description page of Project Settings.


#include "CaveBlocker.h"

#include "DestructibleComponent.h"

// Sets default values
ACaveBlocker::ACaveBlocker()
{
 	// No tick
	PrimaryActorTick.bCanEverTick = false;

	RockDestructableMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("RockDestructableMesh"));
	RootComponent = RockDestructableMesh;
}

// Called when the game starts or when spawned
void ACaveBlocker::BeginPlay()
{
	Super::BeginPlay();
	
}

