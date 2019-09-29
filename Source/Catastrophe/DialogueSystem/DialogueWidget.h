// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

UCLASS()
class CATASTROPHE_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// The dialogue component reference that is binded
	UPROPERTY(BlueprintReadWrite)
	class UDialogueSystemComponent* DialogueComponent;

public:
	/**
	 * Called to bind the dialogue component to the widget
	 * @author James Johnstone
	 * @param _DialogueComponent The dialogue component to bind to the widget
	 */
	void BindDialogueComponent(class UDialogueSystemComponent* _DialogueComponent);
};
