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
	
	UPROPERTY(BlueprintReadWrite, Category = "QTE_Bob")
	class AQteBobLogicHolder* QteEventLogicHolder;

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QTE_Bob")
	void InitializeQteWidget();
	void InitializeQteWidget_Implementation();

};
