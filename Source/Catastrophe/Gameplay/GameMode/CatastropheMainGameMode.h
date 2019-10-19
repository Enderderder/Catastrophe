// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Gameplay/QTE_Bob/QteBobTypes.h"
#include "RespawnSystem/RespawnSystemTypes.h"
#include "CatastropheMainGameMode.generated.h"

/** Delegates */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerAimingSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCaveGameplaySignature);

/**
 * This is the main gameplay gamemode of the game
 */
UCLASS()
class CATASTROPHE_API ACatastropheMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
/** Delegate events */
public:

	UPROPERTY(BlueprintAssignable)
	FPlayerAimingSignature OnPlayerAimingBegin;

	UPROPERTY(BlueprintAssignable)
	FPlayerAimingSignature OnPlayerAimingEnd;

	UPROPERTY(BlueprintAssignable)
	FCaveGameplaySignature OnCaveGameplayBegin;

	UPROPERTY(BlueprintAssignable)
	FCaveGameplaySignature OnCaveGameplayEnd;

	UPROPERTY(BlueprintAssignable)
	FCaveGameplaySignature OnCaveGameplayReset;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay | General")
	class APlayerCharacter* PlayerCharacter;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay | Cave")
	float CaveAdditionSpeedMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay | Cave")
	float CaveJumpVeloOverride = 1500.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay | Cave")
	float CaveJumpVeloOrigin;

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
	 * Called when a quest objective is completed
	 * @author James Johnstone
	 * @param _CurrentObjective The objective that has just been completed
	 * @param _bUnlocksNewQuest Bool which determines if a new quest is unlocked or not
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay | QuestObjectives")
	void OnQuestObjectiveCompletion(class UQuestObjectiveComponent* _CurrentObjective, bool _bUnlocksNewQuest);

	UFUNCTION(BlueprintCallable, Category = "Gameplay | Cave")
	void StartCaveGameplay();

	UFUNCTION(BlueprintCallable, Category = "Gameplay | Cave")
	void EndCaveGameplay();

	UFUNCTION(BlueprintCallable, Category = "Gameplay | Cave")
	void ResetCaveGameplay();

	/** Setter */
	void SetCaveCameraTrack(class ACaveCameraTrack* _cameraTrackActor) {
		CaveCameraTrack = _cameraTrackActor;
	}

	/** Setter End */

	/** Getter */
	FORCEINLINE class ACaveCameraTrack* GetCaveCameraTrack() const { 
		return CaveCameraTrack; }

	/** Getter End */

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay | Cave", meta = (DisplayName = "OnCaveGameplayBegin"))
	void Receive_OnCaveGameplayBegin();

	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay | Cave", meta = (DisplayName = "OnCaveGameplayEnd"))
	void Receive_OnCaveGameplayEnd();

	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay | Cave", meta = (DisplayName = "OnCaveGameplayReset"))
	void Receive_OnCaveGameplayReset();

private:

	/**
	 * Binded event. Called when a qte that initiated by guard completed
	 * @author Richard Wulansari
	 * @param _eventState The state of the event when complete
	 */
	UFUNCTION()
	void OnGuardQteEventComplete(EQteEventState _eventState);

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
