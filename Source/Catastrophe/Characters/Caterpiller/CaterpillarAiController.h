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

	/** The caterpillar its controlling */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CaterpillarAi")
	class ACaterpillar* ControllingCaterpillar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CaterpillarAi")
	class UBehaviorTree* CaterpillarBehaviourTree;


protected:

	/** Called when the controller posses a character */
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnCaterpillarCatachPlayer();


public:

	/**
	 * Start the cave chase
	 * @author Richard Wulansari
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay | Cave")
	void StartChase();

};
