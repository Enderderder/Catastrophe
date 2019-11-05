// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FadeEffectWidget.generated.h"

// Declare delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFadeEffectWidgetSignature);


/**
 * 
 */
UCLASS(Abstract)
class CATASTROPHE_API UFadeEffectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFadeEffectWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FFadeEffectWidgetSignature OnEffectFinish;

public:

	/**
	 * Starts the fade away effect, call "OnEffectFinish" at the end
	 * @author Richard Wulansari
	 * @param _time: Fade away time
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FadeAwayWidget")
	void StartFade(float _time = 1.0f);
	void StartFade_Implementation(float _time = 1.0f);

};
