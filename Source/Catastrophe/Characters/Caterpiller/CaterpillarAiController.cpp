// Fill out your copyright notice in the Description page of Project Settings.


#include "CaterpillarAiController.h"

#include "Classes/BehaviorTree/BlackboardComponent.h"

#include "Caterpillar.h"

#include "DebugUtility/CatastropheDebug.h"

ACaterpillarAiController::ACaterpillarAiController()
{

}

void ACaterpillarAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControllingCaterpillar = Cast<ACaterpillar>(InPawn);
	if (IsValid(ControllingCaterpillar)
		&& CaterpillarBehaviourTree)
	{
		RunBehaviorTree(CaterpillarBehaviourTree);
	}
	else
	{
		const FString msg = "Caterpillar Ai ERROR: The controller is not controlling a Caterpillar";
		CatastropheDebug::OnScreenDebugMsg(-1, 30.0f, FColor::Red, msg);
	}
}

void ACaterpillarAiController::StartChase()
{
	if (Blackboard)
	{
		Blackboard->SetValueAsBool(TEXT("bChasing"), true);
	}
}
