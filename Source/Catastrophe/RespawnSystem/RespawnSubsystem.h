// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance/CatastropheGameInstanceSubsystem.h"
#include "RespawnSystemTypes.h"
#include "RespawnSubsystem.generated.h"

/** Delegates */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelTransitionSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLevelStreamSignatureOneParam, const FLoadStreamingLevelInfo, _info);

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


	/** Multi level loading support */

	FDistrictInfo tempLoadingDistrictInfo;

	FString tempRespawnLocationName = TEXT("DefaultName");

	bool bShouldBlockDuringEachLoad = true;

	int32 tempTotalUnLoadingLevel = 0;

	int32 tempCurrentUnLoadingLevel = 0;

	int32 tempTotalLoadingLevel = 0;

	int32 tempCurrentLoadingLevel = 0;

	TArray<FName> tempLevelsToLoad;

	TArray<FName> tempLevelsToUnload;

	/** =============================== */

public:
	/** Default constructor */
	URespawnSubsystem();

	/** Broadcast event called when a level transition start happening */
	UPROPERTY(BlueprintAssignable, Category = "Respawn System")
	FLevelTransitionSignature OnLevelTransitionStart;

	/** Broadcast event called when a level transition finish happening */
	UPROPERTY(BlueprintAssignable, Category = "Respawn System")
	FLevelTransitionSignature OnLevelTransitionFinish;

	UPROPERTY(BlueprintAssignable, Category = "Respawn System")
	FLevelStreamSignatureOneParam OnLevelLoaded;

	UPROPERTY(BlueprintAssignable, Category = "Respawn System")
	FLevelStreamSignatureOneParam OnLevelUnLoaded;

protected:

	/** All the respawn locations that gets registered */
	UPROPERTY()
	TArray<FDistrictInfo> Districts;

	UPROPERTY()
	TArray<FName> StreamingLevels;

public:
	/* Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/* Implement this for post initialization after all subsystem has initialized */
	virtual void PostInitialize() override;

	/* Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;

	/**
	 * Register levels that will be stored inside this respawn system
	 * @author Richard Wulansari
	 * @param _levelNames: Array of name of levels thats gonna be registered
	 */
	UFUNCTION(BlueprintCallable, Category = "Respawn System")
	void RegisterStreamingLevels(const TArray<FName> _levelNames);

	/**
	 * Register the district with its required level names
	 * @author Richard Wulansari
	 * @param _district: The district that it is register under
	 * @param _levelRequired: The levels that required to load when loading the district
	 */
	UFUNCTION(BlueprintCallable, Category = "Respawn System")
	void RegisterDistrict(EDISTRICT _district, TArray<FName> _levelRequired);

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
	 * Called to unload a level and reload it
	 * @author Richard Wulansari
	 * @param _loadLevelInfo: The level loading information
	 */
	UFUNCTION(BlueprintCallable, Category = "Respawn System")
	void ResetStreamingLevel(FLoadStreamingLevelInfo _loadLevelInfo);

	/**
	 * Called to store spawn locations at certain district
	 * @author Richard Wulansari
	 * @param _districtType
	 * @param _transform
	 * @param _locationName
	 */
	UFUNCTION(BlueprintCallable, Category = "Respawn System")
	void RegisterRespawnLocation(EDISTRICT _districtType, FTransform _transform, FString _locationName);

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
	 * @param _districtType: The location player is going to move to
	 */
	UFUNCTION(BlueprintCallable, Category = "Respawn System")
	void RespawnPlayerAtLocation(EDISTRICT _districtType);

	/**
	 * Teleport the player to the certain district while also load all the level required
	 * @author Richard Wulansari
	 * @param _district: District that needs to be loaded
	 * @param _locationName: The respawn location name
	 */
	UFUNCTION(BlueprintCallable, Category = "Respawn System")
	void RespawnPlayerAtDistrict(EDISTRICT _district, FString _locationName);

	/**
	 * Gets the instance without going through the GameInstance
	 * @author Richard Wulansari
	 * @param _worldContextObject: The context object of the world
	 */
	static URespawnSubsystem* GetInst(const UObject* _worldContextObject);

	/**
	 * Gets the streaming level name given from an actor
	 * @author Richard Wulansari
	 * @param _actor: The context actor
	 * @return The name of the stream level that actor is in
	 * @note This function may return NAME_None
	 */
	UFUNCTION(BlueprintCallable, Category = "Respawn System")
	static FName GetStreamingLevelNameFromActor(class AActor* _actor);


protected:

	/** Called when a level is loaded */
	UFUNCTION()
	void OnStreamLevelLoaded();

	/** Called when a level is unloaded */
	UFUNCTION()
	void OnStreamLevelUnloaded();

	/** Called when a level is unloaded during reset */
	UFUNCTION()
	void OnStreamLevelResetUnloadFinish();

	/** Called when a level is reloaded during reset */
	UFUNCTION()
	void OnStreamLevelResetReloadFinish();


	// Multiple Level loading support

	UFUNCTION()
	void OnDistrictRequireLevelLoaded();

	UFUNCTION()
	void OnDistrictRequireLevelUnloaded();

	void OnDisctrictLoaded(FDistrictInfo _loadingDistrictInfo, FString _respawnLocationName);


private:

	/**
	 * Gets the respawn transform by location name
	 * @author Richard Wulansari
	 * @param _loadingDistrictInfo: The district info
	 * @param _respawnLocationName: Location name
	 * @return The respawn transform
	 */
	FTransform GetRespawnTransform(FDistrictInfo _loadingDistrictInfo, FString _respawnLocationName) const;

};