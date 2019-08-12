// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CaterpillarAiController.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_API ACaterpillarAiController : public AAIController
{
	GENERATED_BODY()

public:
	/** Default constructor */
	ACaterpillarAiController();


protected:





public:

	/**
	 * 
	 * @author Richard Wulansari
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay | Cave")
	void StartChase();

};
