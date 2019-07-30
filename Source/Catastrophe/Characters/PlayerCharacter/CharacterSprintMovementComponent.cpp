// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSprintMovementComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"



// Sets default values for this component's properties
UCharacterSprintMovementComponent::UCharacterSprintMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterSprintMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// Store the character movement component reference for later use
	ACharacter* ownerCharacter = Cast<ACharacter>(GetOwner());
	if (ownerCharacter)
	{
		CharacterMovementComponent = ownerCharacter->GetCharacterMovement();
	}
}


// Called every frame
void UCharacterSprintMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!HasValidData()) return;

	if (bSprinting)
	{

	}
	else
	{

	}



}

void UCharacterSprintMovementComponent::Sprint()
{
	bWantsToSprint = true;
}

void UCharacterSprintMovementComponent::UnSprint()
{
	bWantsToSprint = false;
}

bool UCharacterSprintMovementComponent::HasValidData() const
{

}

