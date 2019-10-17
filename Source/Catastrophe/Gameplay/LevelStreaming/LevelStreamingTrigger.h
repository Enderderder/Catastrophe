// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnSystem/RespawnSubsystem.h"
#include "LevelStreamingTrigger.generated.h"

/**
 * This actor is 
 */
UCLASS()
class CATASTROPHE_API ALevelStreamingTrigger : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* EditorOnlyMesh;
	
public:	
	// Sets default values for this actor's properties
	ALevelStreamingTrigger();

protected:

	/**
	 * The district type of the destination level
	 * @note This is for teleporting the player
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Respawn System")
	EDISTRICT DestinationLevelDistrict = EDISTRICT::HUB;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Respawn System")
	FString RespawnLocationName = TEXT("Default Name");

public:

	UPROPERTY(BlueprintReadWrite, Category = "Respawn System")
	bool bActive = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Called when trigger collides
	 * @author Richard Wulansari
	 */
	UFUNCTION()
	void OnPlayerEnterTrigger(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
