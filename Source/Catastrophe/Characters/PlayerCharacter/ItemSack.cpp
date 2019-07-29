// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSack.h"

// Sets default values for this component's properties
UItemSack::UItemSack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UItemSack::BeginPlay()
{
	Super::BeginPlay();
	
}

void UItemSack::SetItemType(TSubclassOf<class AUseableItem> _NewItemType)
{
	SackItemType = _NewItemType;
}

void UItemSack::AddItem()
{
	if (ItemAmount < MaxItemAmount)
	{
		ItemAmount++;
	}
}

void UItemSack::AddItems(uint8 _Amount)
{
	ItemAmount = (uint8)FMath::Min((int32)(ItemAmount + _Amount), (int32)MaxItemAmount);
}

void UItemSack::FillItemSack()
{
	ItemAmount = MaxItemAmount;
}

bool UItemSack::IsItemSackFull() const
{
	return (ItemAmount == MaxItemAmount);
}

void UItemSack::RemoveItem()
{
	if (ItemAmount > 0)
	{
		ItemAmount--;
	}
}

void UItemSack::RemoveItems(uint8 _Amount)
{
	ItemAmount = (uint8)FMath::Max((int32)(ItemAmount + _Amount), 0);
}

void UItemSack::EmptySack()
{
	ItemAmount = 0;
}

bool UItemSack::IsSackEmpty() const
{
	return (ItemAmount <= 0);
}

void UItemSack::SetItemAmount(uint8 _Amount)
{
	ItemAmount = _Amount;
}

uint8 UItemSack::GetItemAmount() const
{
	return (ItemAmount);
}

void UItemSack::SetSackSize(uint8 _Size)
{
	MaxItemAmount = _Size;
}

uint8 UItemSack::GetSackSize() const
{
	return(MaxItemAmount);
}

bool UItemSack::IsAbleToUse()
{
	return (ItemAmount > 0);
}
