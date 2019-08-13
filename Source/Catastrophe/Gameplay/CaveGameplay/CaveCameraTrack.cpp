// Fill out your copyright notice in the Description page of Project Settings.


#include "CaveCameraTrack.h"

#include "Camera/CameraComponent.h"
#include "Components/SplineComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DebugUtility/CatastropheDebug.h"

#include "Characters/PlayerCharacter/PlayerCharacter.h"

// Sets default values
ACaveCameraTrack::ACaveCameraTrack()
{
 	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	TrackFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TrackFollowCamera"));
	TrackFollowCamera->SetupAttachment(RootComponent);

	CameraTrackSpline = CreateDefaultSubobject<USplineComponent>(TEXT("CameraTrackSpline"));
	CameraTrackSpline->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACaveCameraTrack::BeginPlay()
{
	Super::BeginPlay();

	// Get and store the player character
	ACharacter* character = UGameplayStatics::GetPlayerCharacter(this, 0);
	PlayerCharacter = Cast<APlayerCharacter>(character);
	if (!PlayerCharacter)
	{
		const FString msg = GetName() + " error: Cannot get player character.";
		CatastropheDebug::OnScreenDebugMsg(-1, 30.0f, FColor::Red, msg);
	}
}

// Called every frame
void ACaveCameraTrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCameraFollowingTrack
		&& IsValid(PlayerCharacter))
	{
		// Get the location thats closest to the player on the track
		CurrentCameraLocation = TrackFollowCamera->GetComponentLocation();
		FVector playerWorldLocation = PlayerCharacter->GetActorLocation();
		FVector closestLocationAlongSpline = 
			CameraTrackSpline->FindLocationClosestToWorldLocation(
				playerWorldLocation, ESplineCoordinateSpace::World);

		// Calculate the ahead location of the camera
		float inputKey = CameraTrackSpline->FindInputKeyClosestToWorldLocation(playerWorldLocation);
		int32 truncKey = FMath::TruncToInt(inputKey);
		int32 nextTruncKey = truncKey + 1;
		float disTruncKey = CameraTrackSpline->GetDistanceAlongSplineAtSplinePoint(truncKey);
		float disNextTruncKey = CameraTrackSpline->GetDistanceAlongSplineAtSplinePoint(nextTruncKey);
		float distanceOnKey =
			(inputKey - (float)truncKey) * (disNextTruncKey - disTruncKey) + disTruncKey;

		// Debug message
		{ 
			const FString msg = FString::SanitizeFloat(distanceOnKey);
			CatastropheDebug::OnScreenDebugMsg(-1, 0.0f, FColor::Blue, msg);
		}
		
		float aheadDistance = 
			FMath::Min(distanceOnKey + CameraDistanceAheadOfPlayer, CameraTrackSpline->GetSplineLength()) + 1.0f; // Add 1.0f to be save for later calculation
		FVector aheadLocationOnSpline = 
			CameraTrackSpline->GetLocationAtDistanceAlongSpline(
				aheadDistance, ESplineCoordinateSpace::World);

		FVector previousLocation = CameraTrackSpline->GetLocationAtDistanceAlongSpline(
			aheadDistance - 1.0f, ESplineCoordinateSpace::World);

// 		FRotator TargetCameraRotation = 
// 			UKismetMathLibrary::FindLookAtRotation(aheadLocationOnSpline, previousLocation);


		FVector TargetCameraLocation = 
			FMath::VInterpTo(
				CurrentCameraLocation, aheadLocationOnSpline,
				DeltaTime, CameraInterpSpeed);
		FRotator TargetCameraRotation = 
			UKismetMathLibrary::FindLookAtRotation(
				TargetCameraLocation, playerWorldLocation);

		TrackFollowCamera->SetWorldLocationAndRotation(TargetCameraLocation, TargetCameraRotation);
	}
}

void ACaveCameraTrack::ActivateCameraTrack()
{
	UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(this, 0.5f);
	bCameraFollowingTrack = true;
	if (IsValid(PlayerCharacter))
		PlayerCharacter->CurrentMovementSet = EPlayerMovementSet::CAVECHASE;
}

void ACaveCameraTrack::DeactivateCameraTrack()
{
	UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(PlayerCharacter, 0.5f);
	bCameraFollowingTrack = false;
	if (IsValid(PlayerCharacter))
		PlayerCharacter->CurrentMovementSet = EPlayerMovementSet::NORMAL;
}

