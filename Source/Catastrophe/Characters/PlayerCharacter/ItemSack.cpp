// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSack.h"

// Sets default values
AItemSack::AItemSack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AItemSack::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemSack::AddItem()
{
	if (ItemAmount < MaxItemAmount)
	{
		ItemAmount++;
	}
}

void AItemSack::AddItems(uint8 _Amount)
{
	ItemAmount = (uint8)FMath::Min((int32)(ItemAmount + _Amount), (int32)MaxItemAmount);
}

void AItemSack::FillItemSack()
{
	ItemAmount = MaxItemAmount;
}

bool AItemSack::IsItemSackFull() const
{
	return (ItemAmount == MaxItemAmount);
}

void AItemSack::RemoveItem()
{
	if (ItemAmount > 0)
	{
		ItemAmount--;
	}
}

void AItemSack::RemoveItems(uint8 _Amount)
{
	ItemAmount = (uint8)FMath::Max((int32)(ItemAmount - _Amount), 0);
}

void AItemSack::EmptyItemSack()
{
	ItemAmount = 0;
}

bool AItemSack::IsItemSackEmpty() const
{
	return (ItemAmount <= 0);
}

void AItemSack::SetItemAmount(uint8 _Amount)
{
	ItemAmount = _Amount;
}

uint8 AItemSack::GetItemAmount() const
{
	return (ItemAmount);
}

void AItemSack::SetSackSize(uint8 _Size)
{
	MaxItemAmount = _Size;
}

uint8 AItemSack::GetSackSize() const
{
	return (MaxItemAmount);
}

bool AItemSack::IsAbleToUse()
{
	return (ItemAmount > 0);
}

void AItemSack::UseItem()
{

}
