// Fill out your copyright notice in the Description page of Project Settings.


#include "CatastropheMainGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "Characters/PlayerCharacter/PlayerCharacter.h"
#include "Gameplay/QTE_Bob/QteBobLogicHolder.h"
#include "Gameplay/CaveGameplay/CaveCameraTrack.h"

#include "DebugUtility/CatastropheDebug.h"

void ACatastropheMainGameMode::StartPlay()
{
	Super::StartPlay();
	
	TArray<AActor*> resultActors;
	UGameplayStatics::GetAllActorsOfClass(this, ACaveCameraTrack::StaticClass(), resultActors);
	if (resultActors.Num() == 1)
	{
		CaveCameraTrack = Cast<ACaveCameraTrack>(resultActors[0]);
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
// 	if (!IsValid(CurrentQteEvent))
// 	{
// 		CurrentQteEvent = GetWorld()->SpawnActor<AQteBobLogicHolder>(QteBobEventClass, FTransform::Identity);
// 		if (CurrentQteEvent)
// 		{
// 
// 		}
// 
// 	}
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
