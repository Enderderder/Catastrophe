// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

/**
 * This is a ghost implementation of the player ui widget
 * All the logic implementation will goes into the widget blueprint
 * The purpose of this class is to be able to called by other c++ classes
 */
UCLASS()
class CATASTROPHE_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/**
	 * Called to toggle crosshair visibility
	 * @author Richard Wulansari
	 * @param _bEnable On/Off
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "PlayerWidget")
	void ToggleCrosshair(bool _bEnable);
	virtual void ToggleCrosshair_Implementation(bool _bEnable);

	/**
	 * Called to toggle stamina UI visibility
	 * @author Richard Wulansari
	 * @param _bEnable On/Off
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "PlayerWidget")
	void ToggleStamina(bool _bEnable);
	virtual void ToggleStamina_Implementation(bool _bEnable);

	/**
	 * Called to toggle inventory UI visibility\
	 * @author Richard Wulansari
	 * @param _bEnable On/Off
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "PlayerWidget")
	void ToggleInventory(bool _bEnable);
	virtual void ToggleInventory_Implementation(bool _bEnable);

	/**
	 * Called to show the interaction UI with custom text/action description
	 * @author Richard Wulansari
	 * @param _descriptionText Custom text that will show the result of the interaction (eg. Climb, Knock, Jump In)
	 * @param _actionText Custom text that will show the player action in order to perform the interaction (eg. Press, Hold)
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "PlayerWidget")
	void ShowInteractionUIWithText(const FString& _descriptionText, const FString& _actionText);
	virtual void ShowInteractionUIWithText_Implementation(const FString& _descriptionText, const FString& _actionText);

	/**
	 * Called to hide the interaction UI
	 * @author Richard Wulansari
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "PlayerWidget")
	void HideInteractionUI();
	virtual void HideInteractionUI_Implementation();
};
