// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableUtility.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AThrowableUtility::AThrowableUtility()
{
	PrimaryActorTick.bCanEverTick = false;

	ThrowableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ThrowableMesh"));
	ThrowableMesh->SetCollisionProfileName(TEXT("Throwable"));
	ThrowableMesh->OnComponentHit.RemoveDynamic(this, &AThrowableUtility::OnThrowableMeshHit);
	ThrowableMesh->OnComponentHit.AddDynamic(this, &AThrowableUtility::OnThrowableMeshHit);
	RootComponent = ThrowableMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the mesh has hit an non "ThrowableAffected" object
void AThrowableUtility::OnThrowableHit_Implementation(AActor* _hitActor, UPrimitiveComponent* _hitComp, FVector _normalImpulse, const FHitResult& _hit)
{

}

void AThrowableUtility::OnThrowableHitEffect_Implementation(AActor* _hitActor, FVector _normalImpulse, const FHitResult& _hit)
{

}

void AThrowableUtility::OnThrowableMeshHit(UPrimitiveComponent* _hitComp, AActor* _otherActor, UPrimitiveComponent* _otherComp, FVector _normalImpulse, const FHitResult& _hit)
{
	if (IsValid(_otherActor) && 
		!_otherActor->ActorHasTag(TEXT("ThrowablesUnaffected")) &&
		!_otherActor->ActorHasTag(TEXT("Player")))
	{
		OnThrowableHitEffect(_otherActor, _normalImpulse, _hit);
		OnThrowableHit(_otherActor, _otherComp, _normalImpulse, _hit);
	}
}