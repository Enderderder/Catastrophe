// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnSubsystem.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Engine/LevelStreaming.h"
#include "Engine/Level.h"
#include "GameFramework/SpringArmComponent.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"

#include "Characters/PlayerCharacter/PlayerCharacter.h"
#include "StreamingLevelInterface.h"

#include "DebugUtility/CatastropheDebug.h"


URespawnSubsystem::URespawnSubsystem() 
	: UCatastropheGameInstanceSubsystem()
{}

void URespawnSubsystem::RespawnPlayerAtDistrict_Internal(EDISTRICT _district, FString _locationName)
{
	FDistrictInfo districtInfo = Districts[(int32)_district];

	// Set the temp values
	tempLoadingDistrictInfo = districtInfo;
	tempRespawnLocationName = _locationName;
	tempCurrentLoadingLevel = 0;
	tempCurrentUnLoadingLevel = 0;
	tempLevelsToLoad.Empty();
	tempLevelsToUnload.Empty();

	// Allocate task for levels needs to be unloaded and	loaded
	{
		for (FName levelToLoad : districtInfo.LevelsToLoad)
		{
			ULevelStreaming* streamLevel =
				UGameplayStatics::GetStreamingLevel(this, levelToLoad);
			if (streamLevel)
			{
				tempLevelsToLoad.Add(levelToLoad);
			}
		}

		for (FName levelName : StreamingLevels)
		{
			ULevelStreaming* streamLevel =
				UGameplayStatics::GetStreamingLevel(this, levelName);
			if (streamLevel && streamLevel->IsLevelLoaded())
			{
				tempLevelsToUnload.Add(levelName);
			}
		}
	}

	// Start the unloading and loading task
	if (tempCurrentUnLoadingLevel < tempLevelsToUnload.Num())
	{
		FLatentActionInfo latenInfo;
		latenInfo.CallbackTarget = this;
		latenInfo.UUID = 0;
		latenInfo.Linkage = 0;
		latenInfo.ExecutionFunction = TEXT("OnDistrictRequireLevelUnloaded");
		UGameplayStatics::UnloadStreamLevel(
			this,
			tempLevelsToUnload[tempCurrentUnLoadingLevel],
			latenInfo,
			bShouldBlockDuringEachLoad);
	}
	else if (tempCurrentLoadingLevel < tempLevelsToLoad.Num())
	{
		FLatentActionInfo latenInfo;
		latenInfo.CallbackTarget = this;
		latenInfo.UUID = 0;
		latenInfo.Linkage = 0;
		latenInfo.ExecutionFunction = TEXT("OnDistrictRequireLevelLoaded");
		UGameplayStatics::LoadStreamLevel(
			this,
			tempLevelsToLoad[tempCurrentLoadingLevel],
			true,
			bShouldBlockDuringEachLoad,
			latenInfo);
	}
	else
	{
		OnDisctrictLoaded(tempLoadingDistrictInfo, tempRespawnLocationName);
	}
}

void URespawnSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// Initialize all the disctricts
	for (int32 i = 0; i < (int32)EDISTRICT::COUNT; ++i)
	{
		FDistrictInfo districtInfo;
		districtInfo.RespawnDistrictType = static_cast<EDISTRICT>(i);
		Districts.Add(districtInfo);
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

// Register level names, only valid level name will be registered
void URespawnSubsystem::RegisterStreamingLevels(const TArray<FName> _levelNames)
{
	for (FName levelName : _levelNames)
	{
		if (ULevelStreaming* streamLevel = 
			UGameplayStatics::GetStreamingLevel(this, levelName))
		{
			StreamingLevels.Add(levelName);
		}
	}
}

void URespawnSubsystem::RegisterDistrict(EDISTRICT _district, TArray<FName> _levelRequired)
{
	for (int32 i = 0; i < (int32)EDISTRICT::COUNT; ++i)
	{
		if (static_cast<EDISTRICT>(i) == _district)
		{
			for (FName levelName : _levelRequired)
			{
				ULevelStreaming* streamLevel =
					UGameplayStatics::GetStreamingLevel(this, levelName);
				if (streamLevel)
				{
					Districts[i].LevelsToLoad.Add(levelName);
				}
			}
			break;
		}
	}
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

void URespawnSubsystem::RegisterRespawnLocation(EDISTRICT _districtType, FTransform _transform, FString _locationName)
{
	// Check if the district is valid
	if ((int32)_districtType < 0 || (int32)_districtType >= (int32)EDISTRICT::COUNT)
	{
		CatastropheDebug::OnScreenErrorMsg(TEXT("Failed to register respawn point because the district type is invalid"));
		UE_LOG(LogTemp, Error, 
			TEXT("Failed to register respawn point because the district type is invalid"));
		return;
	}

	// Force the scale to be 1 so the player respawn will never messed up
	_transform.SetScale3D(FVector::OneVector);

	// Store the location
	FRespawnLocationInfo respawnLocationInfo;
	respawnLocationInfo.LocationName = _locationName;
	respawnLocationInfo.RespawnTransform = _transform;
	Districts[(int32)_districtType].RespawnLocations.Add(respawnLocationInfo);
}

FTransform URespawnSubsystem::GetFirstRespawnLocationAtDistrict(EDISTRICT _districtType)
{
	// Check if the district is valid
	if ((int32)_districtType < 0 || (int32)_districtType >= (int32)EDISTRICT::COUNT)
	{
		UE_LOG(LogTemp, Error,
			TEXT("Unable to find respawn location cause the district type is invalid"));
		return FTransform::Identity;
	}

	// Check if there is any locations registered
	TArray<FTransform> respawnPointsInDistrict; //= RespawnPoints[_districtType].RespawnTransforms;
// 	if (respawnPointsInDistrict.Num() <= 0)
// 	{
// 		UE_LOG(LogTemp, Error,
// 			TEXT("Unable to find respawn location cause there is no location in district"));
// 		return FTransform::Identity;
// 	}

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

void URespawnSubsystem::RespawnPlayerAtDistrict(EDISTRICT _district, FString _locationName)
{
	OnLevelTransitionStart.Broadcast();

	FTimerHandle timerHandle;
	FTimerDelegate timerDele;
	timerDele.BindUFunction(this, FName("RespawnPlayerAtDistrict_Internal"), _district, _locationName);
	float timeDilation = UGameplayStatics::GetGlobalTimeDilation(this);
	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDele, 1.0f * timeDilation, false);
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

void URespawnSubsystem::OnDistrictRequireLevelLoaded()
{
	tempCurrentLoadingLevel++;
	if (tempCurrentLoadingLevel < tempLevelsToLoad.Num())
	{
		FLatentActionInfo latenInfo;
		latenInfo.CallbackTarget = this;
		latenInfo.UUID = 0;
		latenInfo.Linkage = 0;
		latenInfo.ExecutionFunction = TEXT("OnDistrictRequireLevelUnloaded");
		UGameplayStatics::LoadStreamLevel(
			this,
			tempLevelsToLoad[tempCurrentLoadingLevel],
			true,
			bShouldBlockDuringEachLoad,
			latenInfo);
	}
	else
	{
		OnDisctrictLoaded(tempLoadingDistrictInfo, tempRespawnLocationName);
	}
}

void URespawnSubsystem::OnDistrictRequireLevelUnloaded()
{
	tempCurrentUnLoadingLevel++;
	if (tempCurrentUnLoadingLevel < tempLevelsToUnload.Num())
	{
		FLatentActionInfo latenInfo;
		latenInfo.CallbackTarget = this;
		latenInfo.UUID = 0;
		latenInfo.Linkage = 0;
		latenInfo.ExecutionFunction = TEXT("OnDistrictRequireLevelUnloaded");
		UGameplayStatics::UnloadStreamLevel(
			this,
			tempLevelsToUnload[tempCurrentUnLoadingLevel],
			latenInfo,
			bShouldBlockDuringEachLoad);
	}
	else if (tempCurrentLoadingLevel < tempLevelsToLoad.Num())
	{
		FLatentActionInfo latenInfo;
		latenInfo.CallbackTarget = this;
		latenInfo.UUID = 0;
		latenInfo.Linkage = 0;
		latenInfo.ExecutionFunction = TEXT("OnDistrictRequireLevelLoaded");
		UGameplayStatics::LoadStreamLevel(
			this,
			tempLevelsToLoad[tempCurrentLoadingLevel],
			true,
			bShouldBlockDuringEachLoad,
			latenInfo);
	}
	else
	{
		OnDisctrictLoaded(tempLoadingDistrictInfo, tempRespawnLocationName);
	}
}

void URespawnSubsystem::OnDisctrictLoaded(FDistrictInfo _loadingDistrictInfo, FString _respawnLocationName)
{
	FTransform respawnTransform = GetRespawnTransform(_loadingDistrictInfo, _respawnLocationName);
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (IsValid(player))
	{
		player->SetActorLocation(respawnTransform.GetLocation());
		player->SetActorRotation(respawnTransform.GetRotation());

		APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(player);
		if (playerCharacter)
			playerCharacter->GetCameraBoom()->SetRelativeRotation(FRotator::ZeroRotator);
	}

	OnLevelTransitionFinish.Broadcast();
}

// Gets the respawn transform by location name
FTransform URespawnSubsystem::GetRespawnTransform(FDistrictInfo _loadingDistrictInfo, FString _respawnLocationName) const
{
	TArray<FRespawnLocationInfo> respawnLocations = _loadingDistrictInfo.RespawnLocations;
	for (int32 i = 0; i < respawnLocations.Num(); ++i)
	{
		if (respawnLocations[i].LocationName == _respawnLocationName)
		{
			return respawnLocations[i].RespawnTransform;
		}
	}

	// If cannot find the transform specified
	// either return the first in the array or otherwise return identity transform
	if (respawnLocations.Num() > 0)
	{
		return respawnLocations[0].RespawnTransform;
	}
	else return FTransform::Identity;
}
