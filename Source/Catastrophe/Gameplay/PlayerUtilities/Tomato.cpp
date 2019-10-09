// Fill out your copyright notice in the Description page of Project Settings.


#include "Tomato.h"

#include "Components/PrimitiveComponent.h"

#include "Characters/GuardCharacter/Guard.h"

// Sets default values
ATomato::ATomato()
{
 	// Set this actor to not call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

}

void ATomato::OnThrowableHit_Implementation(AActor* _hitActor, UPrimitiveComponent* _hitComp, FVector _normalImpulse, const FHitResult& _hit)
{
	// If the tomato hits the head of a guard
	if (_hitActor->IsA<AGuard>())
	{
		if (_hitComp->ComponentHasTag(TEXT("Head")))
		{
			AGuard* guard = Cast<AGuard>(_hitActor);
			guard->SetGuardState(EGuardState::STUNED);
		}
	}

	Destroy();
}

void ATomato::OnThrowableHitEffect_Implementation(AActor* _hitActor, FVector _normalImpulse, const FHitResult& _hit)
{
	
}
