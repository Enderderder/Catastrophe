// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "ItemSack.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

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
	if (SlotsList.Num() > _Position)
	{
		SlotsList[_Position] = _NewItem;
	}
}

void UInventoryComponent::PickupItem(TSubclassOf<class AItemSack> _NewItemType)
{
	for (auto& slot : SlotsList)
	{
		if (slot->IsA(_NewItemType))
		{
			slot->AddItem();
			return;
		}
	}

	AItemSack* newItemSack = GetWorld()->SpawnActor<AItemSack>(_NewItemType, FTransform::Identity);
	if (newItemSack)
	{
		newItemSack->AddItem();
		SlotsList.Add(newItemSack);
	}
}

void UInventoryComponent::PickupItems(TSubclassOf<class AItemSack> _NewItemType, int _Amount)
{
	for (auto& slot : SlotsList)
	{
		if (slot->IsA(_NewItemType))
		{
			slot->AddItems(_Amount);
			return;
		}
	}

	AItemSack* newItemSack = GetWorld()->SpawnActor<AItemSack>(_NewItemType->StaticClass(), FTransform::Identity);
	if (newItemSack)
	{
		newItemSack->AddItems(_Amount);
		SlotsList.Add(newItemSack);
	}
}

class AItemSack* UInventoryComponent::GetItemSack(int _SlotPosition)
{
	if (SlotsList.Num() > _SlotPosition)
	{
		return SlotsList[_SlotPosition];
	}
	return nullptr;
}

class AItemSack* UInventoryComponent::GetCurrentItemSack()
{
	if (SlotsList.Num() > CurrentSelection)
	{
		return SlotsList[CurrentSelection];
	}
	return nullptr;
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
			
			// Deletes Slot if there is no items in the slot
			if (SlotsList[CurrentSelection]->IsItemSackEmpty())
			{
				//GetWorld()->RemoveActor(SlotsList[CurrentSelection], true);
				SlotsList.RemoveAt(CurrentSelection);
			}
		}
	}
}
