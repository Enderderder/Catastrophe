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

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
}

// Called when the game starts or when spawned
void AThrowableProjectileIndicator::BeginPlay()
{
	Super::BeginPlay();
	
	// Initiate all the spline segment mesh components
	for (int32 i = 0; i < NumberOfMeshSegments; ++i)
	{
		USplineMeshComponent* splineMesh = NewObject<USplineMeshComponent>(this);
		splineMesh->SetStaticMesh(IndicatorMesh);
		splineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		splineMesh->SetForwardAxis(ESplineMeshAxis::X);
		SplineMeshes.Add(splineMesh);
	}
}

// Called every frame
void AThrowableProjectileIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThrowableProjectileIndicator::UpdateIndicatorLine(TArray<FVector> _vectors)
{
	SplineComponent->SetSplinePoints(_vectors, ESplineCoordinateSpace::World, true);

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

