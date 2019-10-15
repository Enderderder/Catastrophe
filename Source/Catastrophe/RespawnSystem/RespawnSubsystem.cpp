// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnSubsystem.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Engine/LevelStreaming.h"
#include "Engine/Level.h"
#include "GameFramework/SpringArmComponent.h"

#include "Kismet/KismetMathLibrary.h"

#include "Characters/PlayerCharacter/PlayerCharacter.h"
#include "StreamingLevelInterface.h"

#include "DebugUtility/CatastropheDebug.h"

URespawnSubsystem::URespawnSubsystem() 
	: UCatastropheGameInstanceSubsystem()
{}

void URespawnSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// Add all the locations to the map
	for (int32 i = (int32)EDISTRICT::HUB; i < (int32)EDISTRICT::LOCATIONCOUNT; ++i)
	{
		EDISTRICT districtType = static_cast<EDISTRICT>(i);
		RespawnPoints.Add(districtType);
		RespawnPoints[districtType].District = districtType;
	}
}

void URespawnSubsystem::PostInitialize()
{
	Super::PostInitialize();

}

void URespawnSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
}

void URespawnSubsystem::LoadLevelStreaming(FLoadStreamingLevelInfo _loadLevelInfo)
{
	// Store the temp value
	tempInfo = _loadLevelInfo;
	
	// If the names are same, then go to reset level
	if (_loadLevelInfo.LoadingLevelName == _loadLevelInfo.OriginalLevelName)
	{
		ResetStreamingLevel(_loadLevelInfo);
		return;
	}

	// Load the level normally
	OnLevelTransitionStart.Broadcast();
	FLatentActionInfo latenInfo;
	latenInfo.CallbackTarget = this;
	latenInfo.UUID = 1;
	latenInfo.Linkage = 0;
	latenInfo.ExecutionFunction = TEXT("OnStreamLevelLoaded");
	UGameplayStatics::LoadStreamLevel(
		this,
		_loadLevelInfo.LoadingLevelName,
		true,
		_loadLevelInfo.bBlockOnLoad,
		latenInfo);
}

void URespawnSubsystem::UnloadStreamingLevel(FName _levelName)
{
	FLatentActionInfo latenInfo;
	UGameplayStatics::UnloadStreamLevel(this, _levelName, latenInfo, false);
}

void URespawnSubsystem::ResetStreamingLevel(FLoadStreamingLevelInfo _loadLevelInfo)
{
	// Store the temp value
	tempInfo = _loadLevelInfo;

	// If loading and unloading names are different, abort
	if (_loadLevelInfo.LoadingLevelName != _loadLevelInfo.OriginalLevelName)
	{
		CatastropheDebug::OnScreenErrorMsg(TEXT("RespawnSystem: Cannot reset level, original name and loading name is different"));
		return;
	}

	OnLevelTransitionStart.Broadcast();
	FLatentActionInfo latenInfo;
	latenInfo.CallbackTarget = this;
	latenInfo.UUID = 3;
	latenInfo.Linkage = 0;
	latenInfo.ExecutionFunction = TEXT("OnStreamLevelResetUnloadFinish");
	UGameplayStatics::UnloadStreamLevel(
		this, 
		_loadLevelInfo.OriginalLevelName,
		latenInfo, 
		false);
}

void URespawnSubsystem::RegisterRespawnLocation(EDISTRICT _districtType, FTransform _transform)
{
	// Check if the district is valid
	if ((int32)_districtType < 0 || (int32)_districtType >= (int32)EDISTRICT::LOCATIONCOUNT)
	{
		UE_LOG(LogTemp, Error, 
			TEXT("Failed to register respawn point because the district type is invalid"));
		return;
	}

	// Force the scale to be 1 so the player respawn will never messed up
	_transform.SetScale3D(FVector::OneVector);

	// Store the location
	RespawnPoints[_districtType].RespawnTransforms.Add(_transform);
}

FTransform URespawnSubsystem::GetFirstRespawnLocationAtDistrict(EDISTRICT _districtType)
{
	// Check if the district is valid
	if ((int32)_districtType < 0 || (int32)_districtType >= (int32)EDISTRICT::LOCATIONCOUNT)
	{
		UE_LOG(LogTemp, Error,
			TEXT("Unable to find respawn location cause the district type is invalid"));
		return FTransform::Identity;
	}

	// Check if there is any locations registered
	TArray<FTransform> respawnPointsInDistrict = RespawnPoints[_districtType].RespawnTransforms;
	if (respawnPointsInDistrict.Num() <= 0)
	{
		UE_LOG(LogTemp, Error,
			TEXT("Unable to find respawn location cause there is no location in district"));
		return FTransform::Identity;
	}

	// Give the transform
	return respawnPointsInDistrict[0];
}

void URespawnSubsystem::RespawnPlayerAtLocation(EDISTRICT _districtType)
{
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (IsValid(player))
	{
		FTransform transform = GetFirstRespawnLocationAtDistrict(_districtType);
		player->SetActorLocation(transform.GetLocation());
		player->SetActorRotation(transform.GetRotation());
		
		const FRotator cameraRotation =
			UKismetMathLibrary::FindLookAtRotation(transform.GetLocation(), player->GetActorLocation());
		UGameplayStatics::GetPlayerController(this, 0)->SetControlRotation(cameraRotation);

		APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(player);
		if (playerCharacter)
		{
			
			//playerCharacter->GetCameraBoom()->SetWorldRotation(cameraRotation);

			

		}
	}
}

URespawnSubsystem* URespawnSubsystem::GetInst(const UObject* _worldContextObject)
{
	if (UGameInstance * gameInst
		= UGameplayStatics::GetGameInstance(_worldContextObject))
	{
		return gameInst->GetSubsystem<URespawnSubsystem>();
	}
	return nullptr;
}

FName URespawnSubsystem::GetStreamingLevelNameFromActor(AActor* _actor)
{
	if (IsValid(_actor))
	{
		return _actor->GetLevel()->GetOuter()->GetFName();
	}

	return NAME_None;
}

void URespawnSubsystem::OnStreamLevelLoaded()
{
	OnLevelLoaded.Broadcast(tempInfo);

	if (tempInfo.bTeleportPlayer)
	{
		RespawnPlayerAtLocation(tempInfo.RespawnDistrictType);
	}

	if (tempInfo.bUnloadOriginalLevel)
	{
		FLatentActionInfo latenInfo;
		latenInfo.CallbackTarget = this;
		latenInfo.UUID = 2;
		latenInfo.Linkage = 0;
		latenInfo.ExecutionFunction = TEXT("OnStreamLevelUnloaded");
		UGameplayStatics::UnloadStreamLevel(
			this, 
			tempInfo.OriginalLevelName, 
			latenInfo, 
			tempInfo.bBlockOnLoad);
	}
	else 
	{
		OnLevelTransitionFinish.Broadcast();
	}
}

void URespawnSubsystem::OnStreamLevelUnloaded()
{
	OnLevelUnLoaded.Broadcast(tempInfo);
	OnLevelTransitionFinish.Broadcast();
}

void URespawnSubsystem::OnStreamLevelResetUnloadFinish()
{
	OnLevelUnLoaded.Broadcast(tempInfo);

	FLatentActionInfo latenInfo;
	latenInfo.CallbackTarget = this;
	latenInfo.UUID = 4;
	latenInfo.Linkage = 0;
	latenInfo.ExecutionFunction = TEXT("OnStreamLevelResetReloadFinish");
	UGameplayStatics::LoadStreamLevel(
		this,
		tempInfo.LoadingLevelName,
		true,
		tempInfo.bBlockOnLoad,
		latenInfo);
}

void URespawnSubsystem::OnStreamLevelResetReloadFinish()
{
	OnLevelLoaded.Broadcast(tempInfo);
	OnLevelTransitionFinish.Broadcast();

	if (tempInfo.bTeleportPlayer)
	{
		RespawnPlayerAtLocation(tempInfo.RespawnDistrictType);
	}
}
