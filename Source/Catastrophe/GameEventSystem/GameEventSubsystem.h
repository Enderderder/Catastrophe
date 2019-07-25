// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance/CatastropheGameInstanceSubsystem.h"
#include "GameEventSubsystem.generated.h"

/** Game Events */

/** Game event delegate with no parameter */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameEventDelegate);

/** End */



/**
 * This is s subsystem that passes events around during gameplay
 * Any class can bind event listener and trigger event using this system
 */
UCLASS()
class CATASTROPHE_API UGameEventSubsystem : public UCatastropheGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UGameEventSubsystem();

	/** An event delegate that invoke when the player get caught by the guards */
	FGameEventDelegate OnPlayerGetCaught;

protected:

public:

	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for post initialization after all subsystem has initialized */
	virtual void PostInitialize() override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;
	

	/** Gets the instance without going through the GameInstance */
	static UGameEventSubsystem* GetInst(const UObject* _worldContextObject);
};
