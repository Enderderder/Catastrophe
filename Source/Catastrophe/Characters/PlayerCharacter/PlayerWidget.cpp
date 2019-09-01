// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"

#include "Runtime/UMG/Public/Components/CanvasPanel.h"

#include "Gameplay/QTE_Bob/QteBobWidget.h"

UPlayerWidget::UPlayerWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	

}

void UPlayerWidget::ToggleCrosshair_Implementation(bool _bEnable)
{
	/// Let blueprint do the thing
}

void UPlayerWidget::ToggleStamina_Implementation(bool _bEnable)
{
	/// Let blueprint do the thing
}

void UPlayerWidget::ToggleInventory_Implementation(bool _bEnable)
{
	/// Let blueprint do the thing
}

UQteBobWidget* UPlayerWidget::CreateQteBobWidget_Implementation(class AQteBobLogicHolder* _qteLogicHolder)
{
	/// Let blueprint do the thing
	return nullptr;
}
