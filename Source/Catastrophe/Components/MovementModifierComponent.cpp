// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementModifierComponent.h"

// Sets default values for this component's properties
UMovementModifierComponent::UMovementModifierComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMovementModifierComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMovementModifierComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

