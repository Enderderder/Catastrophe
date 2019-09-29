// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemComponent.h"

#include "GameFramework/Actor.h"



// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	GetOwner()->StaticClass();
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame (Disabled)
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// DISABLED
	// ...
}

