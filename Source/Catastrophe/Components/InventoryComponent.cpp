// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "Characters/PlayerCharacter/UtilitySacks/ItemSack.h"

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

void UInventoryComponent::AddItemType(class AItemSack* _newItem)
{
	ItemSacks.Add(_newItem);
}

void UInventoryComponent::InitialiseItemTypes(TArray<TSubclassOf<class AItemSack>> _items)
{
	for (int i = 0; i < _items.Num(); ++i)
	{
		AItemSack* newItemSack = GetWorld()->SpawnActor<AItemSack>(_items[i], FTransform::Identity);
		if (newItemSack)
		{
			ItemSacks.Add(newItemSack);
		}
	}
}

void UInventoryComponent::ReplaceItemTypeWith(int _position, class AItemSack* _newItem)
{
	if (ItemSacks.Num() > _position)
	{
		ItemSacks[_position] = _newItem;
	}
}

void UInventoryComponent::PickupItem(TSubclassOf<class AItemSack> _newItemType)
{
	for (int i = 0; i < ItemSacks.Num(); ++i)
	{
		if (ItemSacks[i]->IsA(_newItemType))
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

void UInventoryComponent::PickupItems(TSubclassOf<class AItemSack> _newItemType, int _amount)
{
	for (int i = 0; i < ItemSacks.Num(); ++i)
	{
		if (ItemSacks[i]->IsA(_newItemType))
		{
			if (ItemSacks[i]->CanPickup)
			{
				ItemSacks[i]->AddItems(_amount);

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

int32 UInventoryComponent::GetNumOfSlots()
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
	if (CurrentSelection >= 0 && ItemSacks.Num() > CurrentSelection)
	{
		return ItemSacks[CurrentSelection];
	}

	CurrentSelection = 0;
	return nullptr;
}

class AItemSack* UInventoryComponent::GetPreviousItemSack()
{
	int previousItemSack = CurrentSelection;

	if (ItemSacks.Num() > CurrentSelection && CurrentSelection >= 0)
	{
		for (int i = 0; i < ItemSacks.Num() - 1; ++i)
		{
			if (previousItemSack == 0)
			{
				previousItemSack = ItemSacks.Num() - 1;
			}
			else
			{
				previousItemSack--;
			}

			if (!ItemSacks[previousItemSack]->IsItemSackEmpty() && ItemSacks[previousItemSack] != GetNextItemSack())
			{
				return ItemSacks[previousItemSack];
			}
		}
	}
	else
	{
		CurrentSelection = 0;
	}
	return nullptr;
}

class AItemSack* UInventoryComponent::GetNextItemSack()
{
	int nextItemSack = CurrentSelection;

	if (ItemSacks.Num() > CurrentSelection && CurrentSelection >= 0)
	{
		for (int i = 0; i < ItemSacks.Num() - 1; ++i)
		{
			if (nextItemSack == ItemSacks.Num() - 1)
			{
				if (ItemSacks.Num() > 0)
				{
					nextItemSack = 0;
				}
			}
			else
			{
				nextItemSack++;
			}

			if (!ItemSacks[nextItemSack]->IsItemSackEmpty())
			{
				return ItemSacks[nextItemSack];
			}
		}
	}
	else
	{
		CurrentSelection = 0;
	}
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

void UInventoryComponent::UseItem(bool _bAiming)
{
	if (ItemSacks.Num() > CurrentSelection)
	{
		if (ItemSacks[CurrentSelection] != NULL)
		{
			// Check if the player is aiming if aiming is needed
			if (ItemSacks[CurrentSelection]->IsAimingNeeded && !_bAiming) return;

			ItemSacks[CurrentSelection]->UseItem();
			
			// Choose previous available slot if currently selected sack is empty
			if (ItemSacks[CurrentSelection]->IsItemSackEmpty())
			{
				ChoosePreviousItem();
			}
		}

	}
}
