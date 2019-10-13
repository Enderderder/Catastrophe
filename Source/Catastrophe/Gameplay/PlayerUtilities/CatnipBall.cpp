// Fill out your copyright notice in the Description page of Project Settings.


#include "CatnipBall.h"

#include "Components/StaticMeshComponent.h"

#include "DebugUtility/CatastropheDebug.h"

// Sets default values
ACatnipBall::ACatnipBall()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ACatnipBall::OnThrowableHit_Implementation(AActor* _hitActor, UPrimitiveComponent* _hitComp, FVector _normalImpulse, const FHitResult& _hit)
{
	if (_hitActor->ActorHasTag(TEXT("BigFluff")))
	{
		CatastropheDebug::OnScreenDebugMsg(-1, 10.0f, FColor::Cyan, TEXT("U WIN!!!!!"));
	}
	Destroy();
}

void ACatnipBall::OnThrowableHitEffect_Implementation(AActor* _hitActor, FVector _normalImpulse, const FHitResult& _hit)
{
	
}

