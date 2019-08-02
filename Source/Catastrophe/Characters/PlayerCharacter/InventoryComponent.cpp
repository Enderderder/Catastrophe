// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "ItemSack.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentSelection = 0;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UInventoryComponent::AddItemType(class AItemSack* _NewItem)
{
	SlotsList.Add(_NewItem);
}

void UInventoryComponent::ReplaceItemTypeWith(int _Position, class AItemSack* _NewItem)
{
	SlotsList[_Position] = _NewItem;
}

void UInventoryComponent::ChoosePreviousItem()
{
	if (CurrentSelection == 0)
	{
		CurrentSelection = SlotsList.Num() - 1;
	}
	else
	{
		CurrentSelection--;
	}
}

void UInventoryComponent::ChooseNextItem()
{
	if (CurrentSelection == SlotsList.Num() - 1)
	{
		CurrentSelection = 0;
	}
	else
	{
		CurrentSelection++;
	}
}

void UInventoryComponent::UseItem()
{
	if (SlotsList.Num() > CurrentSelection)
	{
		if (SlotsList[CurrentSelection] != NULL)
		{
			SlotsList[CurrentSelection]->UseItem();
		}
	}
}
