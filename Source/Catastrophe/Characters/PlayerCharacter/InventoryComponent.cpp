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
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentSelection = 0;
}

void UInventoryComponent::AddItemType(class AItemSack* _NewItem)
{
	Slots.Add(_NewItem);
}

void UInventoryComponent::InitialiseItemTypes(TArray<TSubclassOf<class AItemSack>> _Items)
{
	for (int i = 0; i < _Items.Num(); ++i)
	{
		AItemSack* newItemSack = GetWorld()->SpawnActor<AItemSack>(_Items[i], FTransform::Identity);
		if (newItemSack)
		{
			Slots.Add(newItemSack);
		}
	}
}

void UInventoryComponent::ReplaceItemTypeWith(int _Position, class AItemSack* _NewItem)
{
	if (Slots.Num() > _Position)
	{
		Slots[_Position] = _NewItem;
	}
}

void UInventoryComponent::PickupItem(TSubclassOf<class AItemSack> _NewItemType)
{
	for (auto& slot : Slots)
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
		Slots.Add(newItemSack);
	}
}

void UInventoryComponent::PickupItems(TSubclassOf<class AItemSack> _NewItemType, int _Amount)
{
	for (auto& slot : Slots)
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
		Slots.Add(newItemSack);
	}
}

bool UInventoryComponent::IsInventoryEmpty()
{
	if (Slots.Num() > 0)
	{
		return false;
	}
	return true;
}

int UInventoryComponent::GetNumOfSlots()
{
	return Slots.Num();
}

class AItemSack* UInventoryComponent::GetItemSack(int _SlotPosition)
{
	if (Slots.Num() > _SlotPosition)
	{
		return Slots[_SlotPosition];
	}
	return nullptr;
}

class AItemSack* UInventoryComponent::GetItemSackOfType(TSubclassOf<class AItemSack> _ItemSackType)
{
	for (auto& slot : Slots)
	{
		if (slot->IsA(_ItemSackType))
		{
			return slot;
		}
	}
	return nullptr;
}

class AItemSack* UInventoryComponent::GetCurrentItemSack()
{
	if (CurrentSelection >= 0)
	{
		if (Slots.Num() > CurrentSelection)
		{
			return Slots[CurrentSelection];
		}
	}
	CurrentSelection = 0;
	return nullptr;
}

class AItemSack* UInventoryComponent::GetPreviousItemSack()
{
	if (CurrentSelection >= 0)
	{
		if (Slots.Num() > CurrentSelection && CurrentSelection >= 0)
		{
			if (CurrentSelection == 0)
			{
				return Slots.Last();
			}
			else
			{
				return Slots[CurrentSelection - 1];
			}
		}
	}
	CurrentSelection = 0;
	return nullptr;
}

class AItemSack* UInventoryComponent::GetNextItemSack()
{
	if (CurrentSelection >= 0)
	{
		if (Slots.Num() > CurrentSelection)
		{
			if (CurrentSelection == Slots.Num() - 1)
			{
				if (Slots.Num() > 0)
				{
					return Slots[0];
				}
			}
			else
			{
				return Slots[CurrentSelection + 1];
			}
		}
	}
	CurrentSelection = 0;
	return nullptr;
}

void UInventoryComponent::ChoosePreviousItem()
{
	if (CurrentSelection == 0)
	{
		CurrentSelection = Slots.Num() - 1;
	}
	else
	{
		CurrentSelection--;
	}
}

void UInventoryComponent::ChooseNextItem()
{
	if (CurrentSelection == Slots.Num() - 1)
	{
		CurrentSelection = 0;
	}
	else
	{
		CurrentSelection++;
	}
}

void UInventoryComponent::UseItem(bool _IsAiming)
{
	if (Slots.Num() > CurrentSelection)
	{
		if (Slots[CurrentSelection] != NULL)
		{
			// Check if the player is aiming if aiming is needed
			if (Slots[CurrentSelection]->IsAimingNeeded && !_IsAiming) return;

			Slots[CurrentSelection]->UseItem();
			
			// Deletes Slot if there is no items in the slot
			if (Slots[CurrentSelection]->IsItemSackEmpty())
			{
				//Slots[CurrentSelection]->Destroy();
				//Slots.RemoveAt(CurrentSelection);
				if (Slots.Num() == CurrentSelection && Slots.Num() > 0)
				{
					ChoosePreviousItem();
				}
			}
		}

	}
}
