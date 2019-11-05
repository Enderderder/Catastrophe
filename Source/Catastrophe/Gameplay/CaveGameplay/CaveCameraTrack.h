// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CaveCameraTrack.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_API ACaveCameraTrack : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TrackFollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USplineComponent* CameraTrackSpline;

	UPROPERTY()
	class APlayerCharacter* PlayerCharacter;

	UPROPERTY()
	bool bCameraFollowingTrack = false;

public:	
	// Sets default values for this actor's properties
	ACaveCameraTrack();

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay | Cave")
	FVector CurrentCameraLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay | Cave")
	float CameraInterpSpeed = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay | Cave")
	float CameraDistanceAheadOfPlayer = 200.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Camera track control */
	UFUNCTION(BlueprintCallable, Category = "Gameplay | Cave")
	void ActivateCameraTrack();
	UFUNCTION(BlueprintCallable, Category = "Gameplay | Cave")
	void DeactivateCameraTrack();
	UFUNCTION(BlueprintCallable, Category = "Gameplay | Cave")
	void SetCameraTrackPause(bool _bPaused);

	/** Getter */
	FORCEINLINE bool IsCameraFollowingTrack() const { return bCameraFollowingTrack; }
	FORCEINLINE class UCameraComponent* GetTrackFollowCamera() const { return TrackFollowCamera; }
	/** Getter End */
};
