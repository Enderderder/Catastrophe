// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Gameplay/QTE_Bob/QteBobTypes.h"
#include "RespawnSystem/RespawnSystemTypes.h"
#include "CatastropheMainGameMode.generated.h"

/** Delegates */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerAimingSignature);

/**
 * This is the main gameplay gamemode of the game
 */
UCLASS()
class CATASTROPHE_API ACatastropheMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable)
	FPlayerAimingSignature OnPlayerAimingBegin;

	UPROPERTY(BlueprintAssignable)
	FPlayerAimingSignature OnPlayerAimingEnd;

protected:

	/** Array of guards thats chasing the player */
	UPROPERTY(BlueprintReadWrite, Category = "Gameplay | General")
	TArray<AActor*> ChasingGuards;

	/** The class reference to the QTE event */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay | QTE_Bob")
	TSubclassOf<class AQteBobLogicHolder> QteBobEventClass;
	
	/**
	 * The currently running QTE event 
	 * @note There should only be one QTE event running at a time
	 */
	UPROPERTY(BlueprintReadWrite, Category = "Gameplay | QTE_Bob")
	class AQteBobLogicHolder* CurrentGuardQteEvent;

	/** The guard thats currently involved in the Guard QTE event */
	UPROPERTY(BlueprintReadWrite, Category = "Gameplay | QTE_Bob")
	class AGuard* QteGuard;

	/**
	 * Records how many times player has successfully QTE
	 * @note This counter resets when the player failed once
	 */
	UPROPERTY(BlueprintReadWrite, Category = "Gameplay | QTE_Bob")
	int32 GuardQteSuccessCounter = 0;

	/** 
	* The Qte range on the first time when player is getting caught
	* @note The range resets when player fails the qte
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay | QTE_Bob")
	float InitialGaurdQteRange = 24.0f;

	/** Actor reference to the cave gameplay camera track */
	UPROPERTY(BlueprintReadWrite, Category = "Gameplay | Cave")
	class ACaveCameraTrack* CaveCameraTrack;

public:

	/** Transition to all actors beginplay */
	virtual void StartPlay() override;

	/** Called every tick */
	virtual void Tick(float DeltaSeconds) override;


	/**
	 * Increase the counter of the number of guard thats chasing the player
	 * @author Richard Wulansari
	 * @param _guard The guard reference
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay | General")
	void AddChasingGuard(AActor* _guard);

	/**
	 * Decrease the counter of the number of guard thats chasing the player
	 * @author Richard Wulansari
	 * @param _guard The guard reference
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay | General")
	void RemoveOneChasingGuard(AActor* _guard);

	/**
	 * Initiate the qte event which will involve the guard and the player character
	 * This will spawn the AQteBobLogicHolder and set the QteGuard as involved guard
	 * @author Richard Wulansari
	 * @param _guard The guard reference
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay | QTE_Bob")
	void InitiateQteBobEvent(class AGuard* _guard);
	virtual void InitiateQteBobEvent_Implementation(class AGuard* _guard);

	/**
	 * Binded event. Called when a qte that initiated by guard completed
	 * @author Richard Wulansari
	 * @param _qteGuard The guard that initiated this qte event
	 */
	UFUNCTION()
	void OnGuardQteEventComplete(EQteEventState _eventState);


	/** Getter */
	FORCEINLINE class ACaveCameraTrack* GetCaveCameraTrack() const { return CaveCameraTrack; }

	/** Getter End */

private:

	/**
	 * Called when player successfully finished a guard QTE event
	 */
	void OnGuardQteSuccess();

	/**
	 * Called when player failed to finished a guard QTE event
	 */
	void OnGuardQteFailed();


public:
	/**
	 * Gets the gamemode with static function call
	 * @author Richard Wulansari
	 * @param _worldContextObject Parameter use to get the gamemode of same world, any world UObject can be
	 * @note If the current gamemode if not ACatastropheMainGameMode, this function will return nullptr
	 */
	static ACatastropheMainGameMode* GetGameModeInst(const UObject* _worldContextObject);








// Console commands
protected:

	UFUNCTION(Exec)
	virtual void Cheat_Teleport(const FString& _levelName, const FString& _districtName);







};
