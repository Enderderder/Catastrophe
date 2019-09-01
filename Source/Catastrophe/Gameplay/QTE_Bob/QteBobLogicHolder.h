// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QteBobTypes.h"
#include "QteBobLogicHolder.generated.h"

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
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "QTE_Bob")
	FOnQteEventCompleteSignature OnQteEventComplete;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QTE_Bob")
	float TimeDilationDuringEvent = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QTE_Bob")
	float DelayBeforeEventStart = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "QTE_Bob")
	EQteEventState EventState = EQteEventState::None;
	
	UPROPERTY(BlueprintReadWrite, Category = "QTE_Bob")
	FQteBobEventInfo EventInfo;

	UPROPERTY(BlueprintReadWrite, Category = "QTE_Bob")
	float CurrentEventTime = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "QTE_Bob")
	class UQteBobWidget* QteBobWidget;

private:

	FTimerHandle EventActualStartHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Binded input function to process when the player hit QTE action
	 * @author Richard Wulansari
	 */
	UFUNCTION()
	void PlayerQteAction();

	/**
	 * Called when the event actually start and enabled input
	 * @author Richard Wulansari
	 */
	UFUNCTION()
	void EventAcutalStart();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Called when the actor gets remove from the world */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * Initiate the QTE event by giving a success range
	 * @author Richard Wulansari
	 * @param _range The range of successful hit zone
	 * @note This function will not define the duration of the event, only will use the default duration value
	 * @see InitiateEvent
	 */
	UFUNCTION(BlueprintCallable, Category = "QTE_Bob")
	void InitiateEventWithRange(float _range);

	/**
	 * Initiate the QTE event by giving a success range and its duration
	 * @author Richard Wulansari
	 * @param _range The range of successful hit zone
	 * @param _duration The duration of the event
	 * @see InitiateEventWithRange
	 */
	UFUNCTION(BlueprintCallable, Category = "QTE_Bob")
	void InitiateEvent(float _range, float _duration);
};
