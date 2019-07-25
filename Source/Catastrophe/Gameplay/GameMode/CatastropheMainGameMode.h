// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CatastropheMainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_API ACatastropheMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay | General")
	int32 ChasingGuardCount = 0;



public:

	/** Transition to all actors beginplay */
	virtual void StartPlay() override;

	/** Called every tick */
	virtual void Tick(float DeltaSeconds) override;


	/**
	 * Increase the counter of the number of guard thats chasing the player
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay | General")
	void AddChasingGuard();

	/**
	 * Decrease the counter of the number of guard thats chasing the player
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay | General")
	void RemoveOneChasingGuard();


	/**
	 * Gets the gamemode with static function call
	 * @author Richard Wulansari
	 * @param _worldContextObject Parameter use to get the gamemode of same world, any world UObject can be
	 * @note If the current gamemode if not ACatastropheMainGameMode, this function will return nullptr
	 */
	static ACatastropheMainGameMode* GetGameModeInst(const UObject* _worldContextObject);

};
