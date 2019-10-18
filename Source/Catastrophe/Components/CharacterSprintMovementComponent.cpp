// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSprintMovementComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/MovementModifierComponent.h"

#include "DebugUtility/CatastropheDebug.h"

// Sets default values for this component's properties
UCharacterSprintMovementComponent::UCharacterSprintMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UCharacterSprintMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// Store the character movement component reference for later use
	CharacterOwner = Cast<ACharacter>(GetOwner());
	if (CharacterOwner)
	{
		MovementModifierComponent = Cast<UMovementModifierComponent>(CharacterOwner->GetComponentByClass(UMovementModifierComponent::StaticClass()));
		if (MovementModifierComponent)
			bUseMovementModifierComponent = true;

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

			if (bUseMovementModifierComponent)
			{
				float reverseModifierValue = bUseConstantSprintSpeed ? 
					-(ConstantSprintSpeed / WalkSpeed) : -(SprintSpeedMultiplier - 1.0f);
				if (MovementModifierComponent)
					MovementModifierComponent->ApplyWalkSpeedModifier(reverseModifierValue);
			}
			else
			{
				CharacterMovementComponent->MaxWalkSpeed = WalkSpeed;
			}

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

			if (bUseMovementModifierComponent)
			{
				float modifierValue = bUseConstantSprintSpeed ?
					(ConstantSprintSpeed / WalkSpeed) : (SprintSpeedMultiplier - 1.0f);
				if (MovementModifierComponent)
					MovementModifierComponent->ApplyWalkSpeedModifier(modifierValue);
			}
			else
			{
				CharacterMovementComponent->MaxWalkSpeed = bUseConstantSprintSpeed ?
					ConstantSprintSpeed : (WalkSpeed * SprintSpeedMultiplier);
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
	return IsValid(CharacterOwner)
		&& (CharacterMovementComponent || MovementModifierComponent);
}

bool UCharacterSprintMovementComponent::IsAbleToSprint() const
{

	if (bSprinting)
	{
		return 
			(CharacterMovementComponent->MovementMode == MOVE_Walking 
				|| CharacterMovementComponent->MovementMode == MOVE_Falling)
			&& GetOwner()->GetVelocity().Size() > 0.0f;
	}
	else
	{
		return
			CharacterMovementComponent->MovementMode == MOVE_Walking
			&& GetOwner()->GetVelocity().Size() > 0.0f;
	}
}

