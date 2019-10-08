// Fill out your copyright notice in the Description page of Project Settings.


#include "CatnipBall.h"

#include "Components/StaticMeshComponent.h"

#include "DebugUtility/CatastropheDebug.h"

// Sets default values
ACatnipBall::ACatnipBall()
{
 	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	CatnipBallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CatnipBallMesh"));
	CatnipBallMesh->SetGenerateOverlapEvents(false);
	CatnipBallMesh->SetCollisionProfileName(TEXT("Throwable"));
	CatnipBallMesh->OnComponentHit.RemoveDynamic(this, &ACatnipBall::OnCatnipBallHit);
	CatnipBallMesh->OnComponentHit.AddDynamic(this, &ACatnipBall::OnCatnipBallHit);
	RootComponent = CatnipBallMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the game starts or when spawned
void ACatnipBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called when the catnip ball has hit something
void ACatnipBall::OnCatnipBallHit(UPrimitiveComponent* _hitComp, AActor* _otherActor, UPrimitiveComponent* _otherComp, FVector _normalImpulse, const FHitResult& _hit)
{
	if (_otherActor->ActorHasTag(TEXT("BigFluff")))
	{
		CatastropheDebug::OnScreenDebugMsg(-1, 10.0f, FColor::Cyan, TEXT("U WIN!!!!!"));



		Destroy();
	}
	else if (!_otherActor->ActorHasTag(TEXT("ThrowablesUnaffected")))
	{
		Destroy();
	}
}

