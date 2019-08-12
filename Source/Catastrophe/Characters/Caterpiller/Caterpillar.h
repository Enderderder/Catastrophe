// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Caterpillar.generated.h"

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
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay | Cave")
	TArray<FVector> CaveFollowPointsWorldSpace;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
