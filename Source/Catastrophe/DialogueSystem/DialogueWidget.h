// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

UCLASS(Abstract)
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

	/**
	 * Called when a sentence appears
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "Dialogue System")
	void OnSentenceAppear();
	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue System", meta = (DisplayName = "OnSentenceAppear"))
	void Receive_OnSentenceAppear();

	/**
	 * Called when a sentence disappears
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "Dialogue System")
	void OnSentenceDisappear();
	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue System", meta = (DisplayName = "OnSentenceDisappear"))
	void Receive_OnSentenceDisappear();

	/**
	 * Called when a sentence changes
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "Dialogue System")
	void OnSentenceChange();
	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue System", meta = (DisplayName = "OnSentenceChange"))
	void Receive_OnSentenceChange();
};
