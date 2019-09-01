// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QteBobTypes.generated.h"

/**
 * Enum state of the qte event
 */
UENUM(BlueprintType)
enum class EQteEventState : uint8
{
	None,
	Pending,
	Success,
	FailedByTimeOut,
	FailedByMissHit
};

/**
 * This struct contain all the stat info of the event
 */
USTRUCT(BlueprintType)
struct FQteBobEventInfo
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EventDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SuccessAreaRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SuccessAreaStart;

	FQteBobEventInfo() :
		EventDuration(0.75f),
		SuccessAreaRange(24.0f),
		SuccessAreaStart(50.0f)
	{}
};