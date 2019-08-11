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
	CharacterOwner = Cast<ACharacter>(GetOwner());
	if (CharacterOwner)
	{
		CharacterMovementComponent = CharacterOwner->GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			WalkSpeed = CharacterMovementComponent->MaxWalkSpeed;
		}
	}
}


// Called every frame
void UCharacterSprintMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Validate data within this class
	if (!HasValidData()) return;

	if (bSprinting)
	{
		// If player either dont want to sprint anymore or dont meet the requirement to sprint
		if (!bWantsToSprint
			|| !IsAbleToSprint()
			|| !bAllowsToSprint)
		{
			bSprinting = false;

			CharacterMovementComponent->MaxWalkSpeed = WalkSpeed;

			OnSprintEnd.Broadcast();
		}
	}
	else
	{
		// If the player both want to sprint and able to sprint
		if (bWantsToSprint
			&& IsAbleToSprint()
			&& bAllowsToSprint)
		{
			bSprinting = true;

			if (bUseConstantSprintSpeed)
			{
				CharacterMovementComponent->MaxWalkSpeed = ConstantSprintSpeed;
			}
			else
			{
				CharacterMovementComponent->MaxWalkSpeed = WalkSpeed * SprintSpeedMultiplier;
			}	

			OnSprintBegin.Broadcast();
		}
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
	const bool hasValidData = 
		CharacterMovementComponent && IsValid(CharacterOwner);

	return hasValidData;
}

bool UCharacterSprintMovementComponent::IsAbleToSprint() const
{
	const bool bAbleToSprint =
		CharacterMovementComponent->MovementMode == MOVE_Walking
		&& GetOwner()->GetVelocity().Size() > 0.0f;

	return bAbleToSprint;
}

