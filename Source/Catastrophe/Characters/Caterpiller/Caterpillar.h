// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Caterpillar.generated.h"

/**
 * The Caterpillar is a character in the cave gamaplay that performs a 
 * nav mesh following to the end of the cave. As its collider collide with the player,
 * it perform a catch player action
 */
UCLASS()
class CATASTROPHE_API ACaterpillar : public ACharacter
{
	GENERATED_BODY()

private:

	/** The trigger box that detect overlap with player character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CatchTriggerBox;

public:
	// Sets default values for this character's properties
	ACaterpillar();

protected:
	
	/**  */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Gameplay | Cave")
	TArray<FVector> CaveFollowPointsWorldSpace;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Called when the catch box triggered
	 */
	UFUNCTION()
	virtual void OnCathchPlayerTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;




};
