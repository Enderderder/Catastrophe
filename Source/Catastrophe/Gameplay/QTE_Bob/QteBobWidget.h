// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QteBobWidget.generated.h"

/**
 * The widget of the QTE mechanic
 * @author Richard Wulansari
 */
UCLASS()
class CATASTROPHE_API UQteBobWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Default class constructor */
	UQteBobWidget(const FObjectInitializer& ObjectInitializer);
	
	/** The range of the success area range for the QTE, goes from 0 - 100 (percent) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite , Category = "QTE_Bob")
	float SuccessRange = 30.0f;
	
	/** 
	 * The starting point of the success area for the QTE, goes from 0 - 100 (percent)
	 * If the starting point of the area plus the range is above 100(percent), it should be forced down to accepted value
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "QTE_Bob")
	float SuccessRangeStartPoint = 50.0f;

	/** The duration of the QTE action, in second */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "QTE_Bob")
	float QteDuration = 3.0f;

public:

	//void ConstructQTE();


};
