// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestWidget.h"


void UQuestWidget::QuestUnlocked()
{
	Receive_QuestUnlocked();

}

void UQuestWidget::QuestCompleted(bool _bUnlocksNewQuest)
{
	Receive_QuestCompleted(_bUnlocksNewQuest);

}

void UQuestWidget::NewQuestObjective(FString _QuestDescription)
{
	Receive_NewQuestObjective(_QuestDescription);

}