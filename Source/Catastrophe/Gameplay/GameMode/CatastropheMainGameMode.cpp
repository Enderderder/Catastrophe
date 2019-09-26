// Fill out your copyright notice in the Description page of Project Settings.


#include "CatastropheMainGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "Characters/PlayerCharacter/PlayerCharacter.h"
#include "Characters/GuardCharacter/Guard.h"
#include "Gameplay/QTE_Bob/QteBobLogicHolder.h"
#include "Gameplay/CaveGameplay/CaveCameraTrack.h"

#include "RespawnSystem/RespawnSubsystem.h"

#include "DebugUtility/CatastropheDebug.h"

void ACatastropheMainGameMode::StartPlay()
{
	Super::StartPlay();
	
	TArray<AActor*> cameraTrackActors;
	UGameplayStatics::GetAllActorsOfClass(this, ACaveCameraTrack::StaticClass(), cameraTrackActors);
	if (cameraTrackActors.Num() == 1)
	{
		CaveCameraTrack = Cast<ACaveCameraTrack>(cameraTrackActors[0]);
	}
	else
	{
		const FString msg = "Insufficient amount of Cave camera track.";
		CatastropheDebug::OnScreenErrorMsg(msg, 30.0f);
	}
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

void ACatastropheMainGameMode::OnGuardQteFailed()
{
	// Reset the success counter and sends player to the jail
	GuardQteSuccessCounter = 0;
	if (QteGuard)
	{
		const FName guardLevelName = URespawnSubsystem::GetStreamingLevelNameFromActor(QteGuard);
		if (guardLevelName != NAME_None)
		{
			FLoadStreamingLevelInfo info;
			info.OriginalLevelName = guardLevelName;
			info.LoadedLevelName = TEXT("ChrisJail");
			info.bUnloadCurrentLevel = true;
			info.bTeleportPlayer = true;
			info.DistrictType = EDISTRICT::JAIL;
			info.bBlockOnLoad = false;
			URespawnSubsystem::GetInst(this)->LoadLevelStreaming(info);
		}
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
		if (index == INDEX_NONE || (EDISTRICT)index >= EDISTRICT::LOCATIONCOUNT)
		{
			CatastropheDebug::OnScreenErrorMsg(TEXT("Invalid district type"), 10.0f);
			return;
		}
		district = (EDISTRICT)index;
	
		FLoadStreamingLevelInfo info;
		
		info.DistrictType = district;
		info.bUnloadCurrentLevel = true;
		URespawnSubsystem::GetInst(this)->LoadLevelStreaming(info);

	}

	if (!enumPtr) district = EDISTRICT::LOCATIONCOUNT;
	else
	{
		
	}





	//CatastropheDebug::OnScreenDebugMsg(-1, 10.0f, FColor::Cyan, enumName);
}
