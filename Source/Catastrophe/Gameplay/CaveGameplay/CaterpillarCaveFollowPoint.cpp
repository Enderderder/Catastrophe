// Fill out your copyright notice in the Description page of Project Settings.


#include "CaterpillarCaveFollowPoint.h"

// Sets default values
ACaterpillarCaveFollowPoint::ACaterpillarCaveFollowPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// This actor never needs to be ticked
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACaterpillarCaveFollowPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACaterpillarCaveFollowPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

