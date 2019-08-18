// Fill out your copyright notice in the Description page of Project Settings.


#include "CatastropheMainGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "Characters/PlayerCharacter/PlayerCharacter.h"
#include "Characters/GuardCharacter/Guard.h"
#include "Gameplay/QTE_Bob/QteBobLogicHolder.h"
#include "Gameplay/CaveGameplay/CaveCameraTrack.h"

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
		CatastropheDebug::OnScreenDebugMsg(-1, 30.0f, FColor::Red, msg);
	}
}

void ACatastropheMainGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	


}

void ACatastropheMainGameMode::AddChasingGuard(AActor* _guard)
{
	CatastropheDebug::OnScreenDebugMsg(-1, 2.0f, FColor::Red, TEXT("Chase!!!!"));

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
	CatastropheDebug::OnScreenDebugMsg(-1, 2.0f, FColor::Red, TEXT("Stop!!!!"));

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
			CurrentGuardQteEvent->InitiateEvent(qteRange);
		}
	}
}

void ACatastropheMainGameMode::OnGuardQteEventComplete(EQteEventState _eventState)
{
	if (_eventState == EQteEventState::Success)
	{

	}
	else if (_eventState == EQteEventState::FailedByMissHit 
		|| _eventState == EQteEventState::FailedByTimeOut)
	{

	}
	else
	{

	}

	switch (_eventState)
	{
	case EQteEventState::Success:
		OnGuardQteSuccess();
		break;
	case EQteEventState::FailedByTimeOut:
		OnGuardQteFailed();
		break;
	case EQteEventState::FailedByMissHit:
		OnGuardQteFailed();
		break;
	default: break;
	}
}

void ACatastropheMainGameMode::OnGuardQteSuccess()
{

}

void ACatastropheMainGameMode::OnGuardQteFailed()
{

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