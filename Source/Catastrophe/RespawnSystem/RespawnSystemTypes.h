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
	JAIL2,
	CAVE,
	HIDEOUT,

	LOCATIONCOUNT // This should always be the last
};