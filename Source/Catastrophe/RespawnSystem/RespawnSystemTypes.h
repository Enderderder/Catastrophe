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

	LOCATIONCOUNT // This should always be the last
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
struct FRespawnLocation
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDISTRICT District;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTransform> RespawnTransforms;

	FRespawnLocation() :
		District(EDISTRICT::HUB)
	{}
};

