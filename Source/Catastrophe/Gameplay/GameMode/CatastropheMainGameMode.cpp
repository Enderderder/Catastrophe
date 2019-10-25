// Fill out your copyright notice in the Description page of Project Settings.


#include "CatastropheMainGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "Characters/PlayerCharacter/PlayerCharacter.h"
#include "Characters/GuardCharacter/Guard.h"
#include "Gameplay/QTE_Bob/QteBobLogicHolder.h"
#include "Gameplay/CaveGameplay/CaveCameraTrack.h"

#include "QuestSystem/Quest.h"
#include "QuestSystem/QuestObjectiveComponent.h"
#include "QuestSystem/QuestWidget.h"

#include "RespawnSystem/RespawnSubsystem.h"
#include "QuestSystem/QuestSubsystem.h"

#include "DebugUtility/CatastropheDebug.h"

void ACatastropheMainGameMode::StartPlay()
{
	Super::StartPlay();
	
	// Store player character reference
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!IsValid(PlayerCharacter))
		CatastropheDebug::OnScreenErrorMsg(TEXT("Gamemode: Cannot get APlayerCharacter"));
}

void ACatastropheMainGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	


}

void ACatastropheMainGameMode::AddChasingGuard(AActor* _guard)
{
	// Only add guard reference if it doesn't already exists
	if (!ChasingGuards.Contains(_guard))
	{
		// Show the spotted effect if this is the first guard chasing
		if (ChasingGuards.Num() == 0)
		{
			APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			if (playerCharacter) playerCharacter->ToggleSpottedAlert(true);
		}

		ChasingGuards.Add(_guard);
	}
}

void ACatastropheMainGameMode::RemoveOneChasingGuard(AActor* _guard)
{
	// Remove the guard reference only if it already exists
	if (ChasingGuards.Contains(_guard))
	{
		ChasingGuards.Remove(_guard);

		// If no more guard chasing, remove the spotted effect on the player
		if (ChasingGuards.Num() == 0)
		{
			APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			if (playerCharacter) playerCharacter->ToggleSpottedAlert(false);
		}
	}
}

void ACatastropheMainGameMode::InitiateQteBobEvent_Implementation(class AGuard* _guard)
{
	if (!IsValid(CurrentGuardQteEvent) && IsValid(_guard))
	{
		CurrentGuardQteEvent = GetWorld()->SpawnActor<AQteBobLogicHolder>(QteBobEventClass, FTransform::Identity);
		if (CurrentGuardQteEvent)
		{
			QteGuard = _guard;
			CurrentGuardQteEvent->OnQteEventComplete.RemoveDynamic(this, &ACatastropheMainGameMode::OnGuardQteEventComplete);
			CurrentGuardQteEvent->OnQteEventComplete.AddDynamic(this, &ACatastropheMainGameMode::OnGuardQteEventComplete);
			
			float qteRange = InitialGaurdQteRange / (float)(GuardQteSuccessCounter + 1);
			CurrentGuardQteEvent->InitiateEventWithRange(qteRange);
		}
	}
}

void ACatastropheMainGameMode::OnQuestObjectiveCompletion(class UQuestObjectiveComponent* _CurrentObjective, bool _bUnlocksNewQuest)
{
	UQuest* CurrentQuest = UQuestSubsystem::GetInst(this)->GetQuestByID(_CurrentObjective->GetQuestOwner()->GetQuestInfo().QuestID);

	if (_CurrentObjective->GetOrder() < CurrentQuest->GetObjectives().Num() - 1)
	{
		PlayerCharacter->GetQuestWidget()->NewQuestObjective(CurrentQuest->GetObjectiveByID(_CurrentObjective->GetOrder() + 1)->GetPopupText());
	}
	else
	{
		PlayerCharacter->GetQuestWidget()->QuestCompleted(_bUnlocksNewQuest);
	}
}

void ACatastropheMainGameMode::StartCaveGameplay()
{
	Receive_OnCaveGameplayBegin();
	OnCaveGameplayBegin.Broadcast();
}

void ACatastropheMainGameMode::EndCaveGameplay()
{
	Receive_OnCaveGameplayEnd();
	OnCaveGameplayEnd.Broadcast();
}

void ACatastropheMainGameMode::ResetCaveGameplay()
{
	Receive_OnCaveGameplayReset();
	OnCaveGameplayReset.Broadcast();
}

void ACatastropheMainGameMode::OnGuardQteEventComplete(EQteEventState _eventState)
{
	if (_eventState == EQteEventState::Success)
	{
		OnGuardQteSuccess();
	}
	else if (_eventState == EQteEventState::FailedByMissHit 
		|| _eventState == EQteEventState::FailedByTimeOut)
	{
		OnGuardQteFailed();
	}
	else
	{
		const FString msg = "QteBobLogicHolder: Event completion happened at wrong event state";
		CatastropheDebug::OnScreenErrorMsg(msg, 10.0f);
	}
}

void ACatastropheMainGameMode::OnGuardQteSuccess()
{
	// Increase the success counter and stun the guard
	GuardQteSuccessCounter++;
	if (QteGuard)
		QteGuard->SetGuardState(EGuardState::STUNED);
}

// Called when player failed to finished a guard QTE event
void ACatastropheMainGameMode::OnGuardQteFailed()
{
	// Reset the success counter and sends player to the jail
	GuardQteSuccessCounter = 0;
	if (QteGuard)
	{
		QteGuard->OnCatchPlayerSuccess();
	}
}

ACatastropheMainGameMode* ACatastropheMainGameMode::GetGameModeInst(const UObject* _worldContextObject)
{
	if (AGameModeBase* gamemode = UGameplayStatics::GetGameMode(_worldContextObject))
	{
		if (ACatastropheMainGameMode* catastropheGamemode = Cast<ACatastropheMainGameMode>(gamemode))
		{
			return catastropheGamemode;
		}
	}

	return nullptr;
}

void ACatastropheMainGameMode::Cheat_Teleport(const FString& _levelName, const FString& _districtName)
{
	EDISTRICT district;
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EDISTRICT"), true);
	if (enumPtr)
	{
		int32 index = enumPtr->GetIndexByName(FName(*_districtName));
		if (index == INDEX_NONE || (EDISTRICT)index >= EDISTRICT::COUNT)
		{
			CatastropheDebug::OnScreenErrorMsg(TEXT("Invalid district type"), 10.0f);
			return;
		}
		district = (EDISTRICT)index;
	
		FLoadStreamingLevelInfo info;
		
		info.RespawnDistrictType = district;
		info.bUnloadOriginalLevel = true;
		URespawnSubsystem::GetInst(this)->LoadLevelStreaming(info);

	}

	if (!enumPtr) district = EDISTRICT::COUNT;
	else
	{
		
	}
	//CatastropheDebug::OnScreenDebugMsg(-1, 10.0f, FColor::Cyan, enumName);
}
