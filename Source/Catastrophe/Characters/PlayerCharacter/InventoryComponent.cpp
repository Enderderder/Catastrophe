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
	ItemSacks.Add(_NewItem);
}

void UInventoryComponent::InitialiseItemTypes(TArray<TSubclassOf<class AItemSack>> _Items)
{
	for (int i = 0; i < _Items.Num(); ++i)
	{
		AItemSack* newItemSack = GetWorld()->SpawnActor<AItemSack>(_Items[i], FTransform::Identity);
		if (newItemSack)
		{
			ItemSacks.Add(newItemSack);
		}
	}
}

void UInventoryComponent::ReplaceItemTypeWith(int _Position, class AItemSack* _NewItem)
{
	if (ItemSacks.Num() > _Position)
	{
		ItemSacks[_Position] = _NewItem;
	}
}

void UInventoryComponent::PickupItem(TSubclassOf<class AItemSack> _NewItemType)
{
	for (int i = 0; i < ItemSacks.Num(); ++i)
	{
		if (ItemSacks[i]->IsA(_NewItemType))
		{
			if (ItemSacks[i]->CanPickup)
			{
				ItemSacks[i]->AddItem();

				// Set currently selected item to this one
				CurrentSelection = i;
			}
			return;
		}
	}

	
}

void UInventoryComponent::PickupItems(TSubclassOf<class AItemSack> _NewItemType, int _Amount)
{
	for (int i = 0; i < ItemSacks.Num(); ++i)
	{
		if (ItemSacks[i]->IsA(_NewItemType))
		{
			if (ItemSacks[i]->CanPickup)
			{
				ItemSacks[i]->AddItems(_Amount);

				CurrentSelection = i;
			}
			return;
		}
	}

	
}

bool UInventoryComponent::IsInventoryEmpty()
{
	if (ItemSacks.Num() > 0)
	{
		return false;
	}
	return true;
}

int UInventoryComponent::GetNumOfSlots()
{
	return ItemSacks.Num();
}

class AItemSack* UInventoryComponent::GetItemSack(int _SlotPosition)
{
	if (ItemSacks.Num() > _SlotPosition)
	{
		return ItemSacks[_SlotPosition];
	}
	return nullptr;
}

class AItemSack* UInventoryComponent::GetItemSackOfType(TSubclassOf<class AItemSack> _ItemSackType)
{
	for (auto& slot : ItemSacks)
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
		if (ItemSacks.Num() > CurrentSelection)
		{
			return ItemSacks[CurrentSelection];
		}
	}
	CurrentSelection = 0;
	return nullptr;
}

class AItemSack* UInventoryComponent::GetPreviousItemSack()
{
	if (CurrentSelection >= 0)
	{
		if (ItemSacks.Num() > CurrentSelection && CurrentSelection >= 0)
		{
			if (CurrentSelection == 0)
			{
				return ItemSacks.Last();
			}
			else
			{
				return ItemSacks[CurrentSelection - 1];
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
		if (ItemSacks.Num() > CurrentSelection)
		{
			if (CurrentSelection == ItemSacks.Num() - 1)
			{
				if (ItemSacks.Num() > 0)
				{
					return ItemSacks[0];
				}
			}
			else
			{
				return ItemSacks[CurrentSelection + 1];
			}
		}
	}
	CurrentSelection = 0;
	return nullptr;
}

void UInventoryComponent::ChoosePreviousItem()
{
	for (int i = 0; i < ItemSacks.Num(); ++i)
	{
		if (CurrentSelection == 0)
		{
			CurrentSelection = ItemSacks.Num() - 1;
		}
		else
		{
			CurrentSelection--;
		}

		if (!ItemSacks[CurrentSelection]->IsItemSackEmpty())
		{
			return;
		}
	}
}

void UInventoryComponent::ChooseNextItem()
{
	for (int i = 0; i < ItemSacks.Num(); ++i)
	{
		if (CurrentSelection == ItemSacks.Num() - 1)
		{
			CurrentSelection = 0;
		}
		else
		{
			CurrentSelection++;
		}

		if (!ItemSacks[CurrentSelection]->IsItemSackEmpty())
		{
			return;
		}
	}
}

void UInventoryComponent::UseItem(bool _IsAiming)
{
	if (ItemSacks.Num() > CurrentSelection)
	{
		if (ItemSacks[CurrentSelection] != NULL)
		{
			// Check if the player is aiming if aiming is needed
			if (ItemSacks[CurrentSelection]->IsAimingNeeded && !_IsAiming) return;

			ItemSacks[CurrentSelection]->UseItem();
			
			// Choose previous available slot if currently selected sack is empty
			if (ItemSacks[CurrentSelection]->IsItemSackEmpty())
			{
				ChoosePreviousItem();
			}
		}

	}
}
