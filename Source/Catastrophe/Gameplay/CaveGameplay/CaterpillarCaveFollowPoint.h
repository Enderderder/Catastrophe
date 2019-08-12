// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CaterpillarCaveFollowPoint.generated.h"

UCLASS()
class CATASTROPHE_API ACaterpillarCaveFollowPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACaterpillarCaveFollowPoint();

protected:

	/**  */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Gameplay | Cave", meta = (MakeEditWidget = "true"))
	TArray<FVector> CaterpillarFollowPoints;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Getter */
	FORCEINLINE TArray<FVector> GetCaterpillarFollowPoints() const { return CaterpillarFollowPoints; }

	/** Getter End */
};
