// Fill out your copyright notice in the Description page of Project Settings.


#include "CaterpillarAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Caterpillar.h"

void UCaterpillarAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	// Store caterpillar character reference
	CaterpillarCharacter = Cast<ACaterpillar>(GetOwningActor());
	if (!CaterpillarCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("CaterpillarCharacter is null, check caterpillar anim class"));
	}

}

void UCaterpillarAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// Check if the correct owner character is still valid
	if (!IsValid(CaterpillarCharacter)) return;

	// Update animation data
	if (UCharacterMovementComponent * CaterpillarMovementComponent
		= CaterpillarCharacter->GetCharacterMovement())
	{
		Speed = CaterpillarMovementComponent->Velocity.Size();
		bInAir = CaterpillarMovementComponent->IsFalling();
	}
}
