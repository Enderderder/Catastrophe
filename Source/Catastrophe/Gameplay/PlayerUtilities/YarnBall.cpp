// Fill out your copyright notice in the Description page of Project Settings.


#include "YarnBall.h"

#include "Components/StaticMeshComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
AYarnBall::AYarnBall()
{
	PrimaryActorTick.bCanEverTick = false;

	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionStimuliSource"));
}

// Called when the yarn ball hit something
void AYarnBall::OnThrowableHit_Implementation(AActor* _hitActor, UPrimitiveComponent* _hitComp, FVector _normalImpulse, const FHitResult& _hit)
{
	GetWorldTimerManager().SetTimer(LifeTimeHandle, this, &AYarnBall::OnYarnBallLifeExpire, Lifetime);
}

void AYarnBall::OnYarnBallLifeExpire()
{
	Receive_OnYarnBallLifeExpire();
	
	Destroy();
}
