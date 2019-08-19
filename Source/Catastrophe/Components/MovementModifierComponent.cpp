// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementModifierComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Engine/World.h"
#include "TimerManager.h"

UMovementModifierComponent::UMovementModifierComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.
	PrimaryComponentTick.bCanEverTick = true;

}

void UMovementModifierComponent::BeginPlay()
{
	Super::BeginPlay();


	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	if (CharacterOwner)
		MovementComponent = CharacterOwner->GetCharacterMovement();
	if (MovementComponent)
	{
		CharacterMaxWalkSpeed = MovementComponent->MaxWalkSpeed;
		CharacterMaxCrouchWalkSpeed = MovementComponent->MaxWalkSpeedCrouched;
	}
}

void UMovementModifierComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Clean up timer handles
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UMovementModifierComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check data validation before further procedure
	if (!ValidateData())
		return;

	if (HasModifierValueChanged())
	{
		// Update the movement component
		MovementComponent->MaxWalkSpeed = CharacterMaxWalkSpeed * FMath::Max(0.0f, WalkSpeedModifier);
		MovementComponent->MaxWalkSpeedCrouched = CharacterMaxCrouchWalkSpeed * FMath::Max(0.0f, CrouchSpeedModifier);

		// Update the recorded precvious value
		PreviousWalkSpeedModifier = WalkSpeedModifier;
		PreviousCrouchSpeedModifier = CrouchSpeedModifier;
	}
}

void UMovementModifierComponent::ApplySpeedModifierToAll(float _modifyValue)
{
	ApplyWalkSpeedModifier(_modifyValue);
	ApplyCrouchSpeedModifier(_modifyValue);
}

void UMovementModifierComponent::ApplySpeedModifierWithDurationToAll(float _modifyValue, float _duration)
{
	ApplyWalkSpeedModifierWithDuration(_modifyValue, _duration);
	ApplyCrouchSpeedModifierWithDuration(_modifyValue, _duration);
}

void UMovementModifierComponent::ApplyWalkSpeedModifier(float _modifyValue)
{
	WalkSpeedModifier += _modifyValue;
}

void UMovementModifierComponent::ApplyWalkSpeedModifierWithDuration(float _modifyValue, float _duration)
{
	ApplyWalkSpeedModifier(_modifyValue);

	// Sets timer function to reverse the modifier change after given duration
	FTimerHandle reverseModifyTimerHandle;
	FTimerDelegate reverseModifierChangeDel;
	reverseModifierChangeDel.BindUFunction(this, FName("ApplyWalkSpeedModifier"), _modifyValue);
	GetWorld()->GetTimerManager().SetTimer(reverseModifyTimerHandle, reverseModifierChangeDel, _duration, false);
}

void UMovementModifierComponent::ApplyCrouchSpeedModifier(float _modifyValue)
{
	CrouchSpeedModifier += _modifyValue;
}

void UMovementModifierComponent::ApplyCrouchSpeedModifierWithDuration(float _modifyValue, float _duration)
{
	ApplyCrouchSpeedModifier(_modifyValue);

	// Sets timer function to reverse the modifier change after given duration
	FTimerHandle reverseModifyTimerHandle;
	FTimerDelegate reverseModifierChangeDel;
	reverseModifierChangeDel.BindUFunction(this, FName("ApplyCrouchSpeedModifier"), _modifyValue);
	GetWorld()->GetTimerManager().SetTimer(reverseModifyTimerHandle, reverseModifierChangeDel, _duration, false);
}

bool UMovementModifierComponent::ValidateData()
{
	return IsValid(MovementComponent);
}

bool UMovementModifierComponent::HasModifierValueChanged()
{
	return WalkSpeedModifier != PreviousWalkSpeedModifier 
		|| CrouchSpeedModifier != PreviousCrouchSpeedModifier;
}

