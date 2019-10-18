// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestWidget.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Quest Widget")
	void QuestUnlocked();
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest Widget", meta = (DisplayName = "OnQuestUnlocked"))
	void Receive_QuestUnlocked();

	UFUNCTION(BlueprintCallable, Category = "Quest Widget")
	void QuestCompleted(bool _bUnlocksNewQuest);
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest Widget", meta = (DisplayName = "OnQuestCompleted"))
	void Receive_QuestCompleted(bool _bUnlocksNewQuest);

	UFUNCTION(BlueprintCallable, Category = "Quest Widget")
	void NewQuestObjective(FString _QuestDescription);
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest Widget", meta = (DisplayName = "OnNewQuestObjective"))
	void Receive_NewQuestObjective(const FString& _QuestDescription);
};
