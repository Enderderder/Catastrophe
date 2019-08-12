// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QteBobLogicHolder.generated.h"

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
 * Contains information of the QTE event
 */
USTRUCT(BlueprintType)
struct FQteEventData
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQteEventState EventState;
	


	
	FQteEventData() : 
		EventState(EQteEventState::None)
	{}
};

/** Delegate signature for Qte event */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQteEventCompleteSignature, EQteEventState, _eventState);

/**
 * 
 */
UCLASS()
class CATASTROPHE_API AQteBobLogicHolder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQteBobLogicHolder();

	/** Event broadcast when an qte event completed */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "General | QTE_Bob")
	FOnQteEventCompleteSignature OnQteEventComplete;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
