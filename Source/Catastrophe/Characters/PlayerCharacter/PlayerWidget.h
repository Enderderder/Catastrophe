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
	UPlayerWidget(const FObjectInitializer& ObjectInitializer);
	

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UCanvasPanel* MainCanvasPanel;

protected:

	virtual void NativeConstruct() override;

public:

	/**
	 * Called to toggle crosshair visibility
	 * @author Richard Wulansari
	 * @param _bEnable On/Off
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PlayerWidget")
	void ToggleCrosshair(bool _bEnable);
	virtual void ToggleCrosshair_Implementation(bool _bEnable);

	/**
	 * Called to toggle stamina UI visibility
	 * @author Richard Wulansari
	 * @param _bEnable On/Off
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PlayerWidget")
	void ToggleStamina(bool _bEnable);
	virtual void ToggleStamina_Implementation(bool _bEnable);

	/**
	 * Called to toggle inventory UI visibility\
	 * @author Richard Wulansari
	 * @param _bEnable On/Off
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PlayerWidget")
	void ToggleInventory(bool _bEnable);
	virtual void ToggleInventory_Implementation(bool _bEnable);

	/**
	 * Set the visibility of the interaction ui
	 * @author Richard Wulansari
	 * @param _visibility
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PlayerWidget | Interaction")
	void SetInteractionUiVisible(bool _visibility);

	/**
	 * Update the interaction ui
	 * @author Richard Wulansari
	 * @param _interactableComp The interactable component that holds all the info
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PlayerWidget | Interaction")
	void UpdateInteractionUi(class UInteractableComponent* _interactableComp);

	/**
	 * Called to create a QTE widget with its logic holder
	 * @author Richard Wulansari
	 * @param _qteLogicHolder The logic holder of the QTE event which contains all information of the event
	 * @return The created widget object
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PlayerWidget")
	class UQteBobWidget* CreateQteBobWidget(class AQteBobLogicHolder* _qteLogicHolder);
	class UQteBobWidget* CreateQteBobWidget_Implementation(class AQteBobLogicHolder* _qteLogicHolder);


};
