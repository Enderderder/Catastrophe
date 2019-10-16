#pragma once

#include "CoreMinimal.h"
#include "RespawnSystemTypes.generated.h"

/**
 * This is a enum collection of all the posible respawn destrict in the game
 */
UENUM(BlueprintType)
enum class EDISTRICT : uint8
{
	HUB = 0,
	MARKET,
	HOLDINGCELL,
	JAIL,
	CAVE,
	HIDEOUT,
	BIGFLUFF,

	COUNT // This should always be the last
};

/**
 *
 */
USTRUCT(BlueprintType)
struct FRespawnLocationInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LocationName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform RespawnTransform;

	FRespawnLocationInfo() :
		LocationName(TEXT("DefaultName")),
		RespawnTransform(FTransform::Identity)
	{}
};

/**
 *
 */
USTRUCT(BlueprintType)
struct FLoadStreamingLevelInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName OriginalLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LoadingLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUnloadOriginalLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTeleportPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDISTRICT RespawnDistrictType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBlockOnLoad;

	FLoadStreamingLevelInfo() :
		OriginalLevelName(TEXT("DefaultName")),
		LoadingLevelName(TEXT("DefaultName")),
		bUnloadOriginalLevel(true),
		bTeleportPlayer(true),
		RespawnDistrictType(EDISTRICT::HUB),
		bBlockOnLoad(false)
	{}
};

/**
 *
 */
USTRUCT(BlueprintType)
struct FDistrictInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDISTRICT RespawnDistrictType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> LevelsToLoad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRespawnLocationInfo> RespawnLocations;

	FDistrictInfo() :
		RespawnDistrictType(EDISTRICT::HUB)
	{}
};