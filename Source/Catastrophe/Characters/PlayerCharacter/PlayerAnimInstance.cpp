// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "PlayerCharacter.h"

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	// Store player character reference
	CustomPlayerCharacter = Cast<APlayerCharacter>(GetOwningActor());
	if (!CustomPlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("CustomPlayerCharacter is null, check player character anim class"));
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	// Verify again after parent function call to make sure
	if (!IsValid(CustomPlayerCharacter)) return;
	
	// Update animation data
	if (UCharacterMovementComponent* PlayerMovementComponent 
		= CustomPlayerCharacter->GetCharacterMovement())
	{
		Speed = PlayerMovementComponent->Velocity.Size();
		bInAir = PlayerMovementComponent->IsFalling();
		bCrouch = PlayerMovementComponent->IsCrouching();
	}
}
