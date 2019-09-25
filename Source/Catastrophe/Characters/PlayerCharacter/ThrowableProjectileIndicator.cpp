// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableProjectileIndicator.h"

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/StaticMesh.h"


// Sets default values
AThrowableProjectileIndicator::AThrowableProjectileIndicator()
{
 	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	SetActorHiddenInGame(true);

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = SplineComponent;
}

// Called when the game starts or when spawned
void AThrowableProjectileIndicator::BeginPlay()
{
	Super::BeginPlay();

	// Initiate all the spline segment mesh components
	for (int32 i = 0; i < NumberOfMeshSegments; ++i)
	{
		USplineMeshComponent* splineMesh = NewObject<USplineMeshComponent>(this);
		splineMesh->RegisterComponent();
		splineMesh->SetMobility(EComponentMobility::Movable);
		splineMesh->SetForwardAxis(ESplineMeshAxis::X);
		splineMesh->SetStaticMesh(IndicatorMesh);
		splineMesh->SetMaterial(0, IndicatorMaterial);
		splineMesh->SetGenerateOverlapEvents(false);
		splineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		splineMesh->SetCastShadow(false);	
		splineMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMeshes.Add(splineMesh);
	}
}

// Called every frame
void AThrowableProjectileIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SplineComponent->GetNumberOfSplinePoints() > 0 &&
		bRenderingSpline)
	{
		float segmentLength = SplineComponent->GetSplineLength() / NumberOfMeshSegments;
		for (int32 i = 0; i < NumberOfMeshSegments; ++i)
		{
			FVector startPos = SplineComponent->GetLocationAtDistanceAlongSpline(segmentLength * i, ESplineCoordinateSpace::World);
			FVector startTangent = SplineComponent->GetTangentAtDistanceAlongSpline(segmentLength * i, ESplineCoordinateSpace::World);
			FVector endPos = SplineComponent->GetLocationAtDistanceAlongSpline(segmentLength * (i + 1), ESplineCoordinateSpace::World);;
			FVector endTangent = SplineComponent->GetTangentAtDistanceAlongSpline(segmentLength * (i + 1), ESplineCoordinateSpace::World);;

			SplineMeshes[i]->SetStartAndEnd(startPos, startTangent, endPos, endTangent);
		}
	}
}

// Update the points in the spline
void AThrowableProjectileIndicator::UpdateIndicatorLine(TArray<FVector> _vectors)
{
	SplineComponent->ClearSplinePoints(false);
	SplineComponent->SetSplinePoints(_vectors, ESplineCoordinateSpace::World, true);
}

void AThrowableProjectileIndicator::SetIndicatorEnabled(bool _enabled)
{
	bRenderingSpline = _enabled;
	SetActorHiddenInGame(_enabled);
}
