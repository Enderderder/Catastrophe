// Fill out your copyright notice in the Description page of Project Settings.


#include "CatastropheMainGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "Characters/PlayerCharacter/PlayerCharacter.h"

#include "DebugUtility/CatastropheDebug.h"

void ACatastropheMainGameMode::StartPlay()
{
	Super::StartPlay();
	


}

void ACatastropheMainGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	


}

void ACatastropheMainGameMode::AddChasingGuard()
{
	CatastropheDebug::OnScreenDebugMsg(-1, 2.0f, FColor::Red, TEXT("Chase!!!!"));

	if (ChasingGuardCount == 0)
	{
		APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (playerCharacter) playerCharacter->ToggleSpottedAlert(true);
	}

	ChasingGuardCount++;
}

void ACatastropheMainGameMode::RemoveOneChasingGuard()
{
	CatastropheDebug::OnScreenDebugMsg(-1, 2.0f, FColor::Red, TEXT("Stop!!!!"));

	ChasingGuardCount = FMath::Max(0, ChasingGuardCount--);
	if (ChasingGuardCount == 0)
	{
		APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (playerCharacter) playerCharacter->ToggleSpottedAlert(false);
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
