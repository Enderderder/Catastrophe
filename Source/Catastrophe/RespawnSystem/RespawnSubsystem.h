// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance/CatastropheGameInstanceSubsystem.h"
#include "RespawnSubsystem.generated.h"

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
	FName LoadedLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUnloadCurrentLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTeleportPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDISTRICT DistrictType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBlockOnLoad;

	FLoadStreamingLevelInfo() :
		OriginalLevelName(TEXT("DefaultName")),
		LoadedLevelName(TEXT("DefaultName")),
		bUnloadCurrentLevel(true),
		bTeleportPlayer(true),
		DistrictType(EDISTRICT::HUB),
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


/** Delegates */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelTransitionSignature);

/**
 * This system controls the spawn of the player
 */
UCLASS()
class CATASTROPHE_API URespawnSubsystem : public UCatastropheGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:

	/** This is a temporary info store */
	FLoadStreamingLevelInfo tempInfo;

public:
	/** Default constructor */
	URespawnSubsystem();

	/** Broadcast event called when a level transition start happening */
	UPROPERTY(BlueprintAssignable, Category = "Respawn System")
	FLevelTransitionSignature OnLevelStartLoad;

	/** Broadcast event called when a level transition finish happening */
	UPROPERTY(BlueprintAssignable, Category = "Respawn System")
	FLevelTransitionSignature OnLevelFinsihLoad;


protected:

	/** All the respawn locations that gets registered */
	UPROPERTY()
	TMap<EDISTRICT, FRespawnLocation> RespawnPoints;

public:
	/* Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/* Implement this for post initialization after all subsystem has initialized */
	virtual void PostInitialize() override;

	/* Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;

	/**
	 * Load the level inside the level streaming world
	 * @author Richard Wulansari
	 * @param _player The player reference that needs to be provided
	 * @param _loadLevelInfo Information that needs to be filled in order to load level
	 * @note Do not call this function before all BeginPlay() finished
	 */
	UFUNCTION(BlueprintCallable, Category = "Respawn System")
	void LoadLevelStreaming(FLoadStreamingLevelInfo _loadLevelInfo);

	/**
	 * Unload certain level by name
	 * @author Richard Wulansari
	 * @param _levelName The name of the streaming level
	 */
	UFUNCTION(BlueprintCallable, Category = "Respawn System")
	void UnloadStreamingLevel(FName _levelName);

	/**
	 * Called to store spawn locations at certain district
	 * @author Richard Wulansari
	 * @param _districtType
	 * @param _transform
	 */
	UFUNCTION(BlueprintCallable, Category = "Respawn System")
	void RegisterRespawnLocation(EDISTRICT _districtType, FTransform _transform);

	/**
	 * Gets the first respawn location at provided district type
	 * @author Richard Wulansari
	 * @param _districtType
	 * @note If there is no respawn location avaliable, return zero transform
	 * @note Cause this will be the repsawn transform, the scale will forced to set to 1
	 */
	UFUNCTION(BlueprintCallable, Category = "Respawn System")
	FTransform GetFirstRespawnLocationAtDistrict(EDISTRICT _districtType);

	/**
	 * Relocate the player to a district
	 * @author Richard Wulansari
	 * @param _districtType The location player is going to move to
	 */
	UFUNCTION(BlueprintCallable, Category = "Respawn System")
	void RespawnPlayerAtLocation(EDISTRICT _districtType);

	/**
	 * Gets the instance without going through the GameInstance
	 * @author Richard Wulansari
	 * @param _worldContextObject The context object of the world
	 */
	static URespawnSubsystem* GetInst(const UObject* _worldContextObject);

	/**
	 * Gets the streaming level name given from an actor
	 * @author Richard Wulansari
	 * @param _actor The context actor
	 */
	UFUNCTION(BlueprintCallable, Category = "Respawn System")
	static FName GetStreamingLevelNameFromActor(class AActor* _actor);


protected:

	/** Called when an level is loaded */
	UFUNCTION()
	void OnStreamLevelLoaded();

	/** Called when an level is unloaded */
	UFUNCTION()
	void OnStreamLevelUnloaded();

};
