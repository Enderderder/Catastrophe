// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardAiController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Classes/BehaviorTree/BlackboardComponent.h"
// #include "NavigationSystem/Public/NavigationSystem.h"
// #include "NavigationSystem/Public/NavigationPath.h"

#include "Guard.h"
#include "Gameplay/PlayerUtilities/YarnBall.h"

#include "DebugUtility/CatastropheDebug.h"


AGuardAiController::AGuardAiController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	//PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AGuardAiController::TargetPerceptionUpdate);
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AGuardAiController::PerceptionUpdate);

	SightDefaultConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightDefaultConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightDefaultConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightDefaultConfig->DetectionByAffiliation.bDetectNeutrals = true;
	PerceptionComponent->ConfigureSense(*SightDefaultConfig);

	HearingDefaultConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	HearingDefaultConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingDefaultConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingDefaultConfig->DetectionByAffiliation.bDetectNeutrals = true;
	PerceptionComponent->ConfigureSense(*HearingDefaultConfig);
}

void AGuardAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	// Sets the reference of the guard
	ControllingGuard = Cast<AGuard>(InPawn);
	if (IsValid(ControllingGuard))
	{
		ControllingGuard->SetGuardControllerRef(this);

		// Runs the behaviour tree of guard
		if (GuardBehaviourTree)
			RunBehaviorTree(GuardBehaviourTree);

		if (Blackboard)
		{
			// Check if guard is capable of doing patrol behaviour
			// If not, return to stationary
			bool bCapableOfPatrolling =
				ControllingGuard->bHasPatrolBehaviour &&
				ControllingGuard->PatrolLocations.Num() > 0;
			if (bCapableOfPatrolling)
			{
				// Sets the origin location of the patrol location
				Blackboard->SetValueAsVector(
					TEXT("PatrolOriginLocation"),
					ControllingGuard->PatrolLocations[0] + ControllingGuard->GetActorLocation());
			}
			else if (ControllingGuard->GetPreferNeutralState() == EGuardState::PATROLLING)
			{
				ControllingGuard->SetPreferNeutralState(EGuardState::STATIONARY);
			}
			
			// Sets the default state of the guard
			//ControllingGuard->SetGuardState(ControllingGuard->PreferNeutralState);
			Blackboard->SetValueAsEnum(
				TEXT("PreferNeutralState"), (uint8)(ControllingGuard->PreferNeutralState));
			Blackboard->SetValueAsVector(
				TEXT("OriginLocation"), ControllingGuard->GetActorLocation());
			Blackboard->SetValueAsRotator(
				TEXT("OriginRotation"), ControllingGuard->GetActorRotation());
			Blackboard->SetValueAsBool(
				TEXT("bCapableOfPatrolling"), bCapableOfPatrolling);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Guard controller is not controlling a guard"));
	}
}

void AGuardAiController::PerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* actor : UpdatedActors)
	{
		const FString msg = TEXT("PerceptionUpdated");
		CatastropheDebug::OnScreenDebugMsg(-1, 2.0f, FColor::Cyan, msg);

		FActorPerceptionBlueprintInfo actorPerceptionInfo;
		if (PerceptionComponent->GetActorsPerception(actor, actorPerceptionInfo))
		{
			for (int32 index = 0; index < actorPerceptionInfo.LastSensedStimuli.Num(); ++index)
			{
				if (actorPerceptionInfo.LastSensedStimuli[index].IsValid())
				{
					switch (index)
					{
					case 0: // Sight perception updated
						OnSightPerceptionUpdate(actor, actorPerceptionInfo.LastSensedStimuli[index]);
						break;

					case 1: // Hearing perception updated
						OnHearingPerceptionUpdate(actor, actorPerceptionInfo.LastSensedStimuli[index]);
						break;

					default:
						UE_LOG(LogTemp, Warning, TEXT("There is no third sense lol wtf"));
						break;
					}
				}
			}
		}
	}
}

void AGuardAiController::OnSightPerceptionUpdate(AActor* _actor, FAIStimulus _stimulus)
{
	// If updated actor is player
	if (_actor->ActorHasTag(TEXT("Player")))
	{
		if (_stimulus.WasSuccessfullySensed())
		{
			ControllingGuard->bPlayerInSight = true;
			if (!ControllingGuard->bPlayerWasInSight)
			{
				ControllingGuard->bPlayerWasInSight = true;
				Blackboard->SetValueAsBool(
					TEXT("bHasSightOnPlayer"), true);
			}
		}
		else if (ControllingGuard->bPlayerInSight)
		{
			ControllingGuard->bPlayerInSight = false;
			Blackboard->SetValueAsBool(
				TEXT("bHasSightOnPlayer"), false);
			Blackboard->SetValueAsBool(
				TEXT("bPlayerWasInSight"), true);
			Blackboard->SetValueAsVector(
				TEXT("PlayerlastSeenLocation"), _stimulus.StimulusLocation);
		}
		else
		{
			// Nothing happened yet
		}
	}
	else if (_actor->IsA<AYarnBall>())
	{
		// Make guard move to the yarn ball location
		Blackboard->SetValueAsVector(TEXT("PointOfInterest"), _actor->GetActorLocation());
		ControllingGuard->SetGuardState(EGuardState::INVESTATING);
	}

	// Calls the guard character version of the function
	ControllingGuard->OnSightPerceptionUpdate(_actor, _stimulus);

}

void AGuardAiController::OnHearingPerceptionUpdate(AActor* _actor, FAIStimulus _stimulus)
{
	// TODO: If not chasing player which is the higher priority task,
	// investigate whatever the sound is

	// If heard player making noise
	if (_actor->ActorHasTag(TEXT("Player")))
	{
		if (_stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("I can hear uuuuu"));
		}
	}

	// Calls the guard character version of the function
	ControllingGuard->OnHearingPerceptionUpdate(_actor, _stimulus);
}

bool AGuardAiController::ModifySightRange(float _newSightRange, float _losingSightRange)
{
	FAISenseID sightSenseID = UAISense::GetSenseID(UAISense_Sight::StaticClass());
	if (!sightSenseID.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get sight sense ID"));
		return false;
	}

	if (!PerceptionComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("This controller dont have perception component"));
		return false;
	}
	UAISenseConfig_Sight* sightConfig = 
		Cast<UAISenseConfig_Sight>(PerceptionComponent->GetSenseConfig(sightSenseID));
	if (!sightConfig)
	{
		UE_LOG(LogTemp, Warning, TEXT("No sight config found"));
		return false;
	}
	
	// Modify values and tell the config to be updated to the system
	sightConfig->SightRadius = _newSightRange;
	sightConfig->LoseSightRadius = _newSightRange + _losingSightRange;
	PerceptionComponent->RequestStimuliListenerUpdate();

	return true;
}
