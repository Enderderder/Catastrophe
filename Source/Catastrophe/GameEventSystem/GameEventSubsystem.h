// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance/CatastropheGameInstanceSubsystem.h"
#include "GameEventSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_API UGameEventSubsystem : public UCatastropheGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UGameEventSubsystem();

protected:

public:

	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for post initialization after all subsystem has initialized */
	virtual void PostInitialize() override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;
	
};
