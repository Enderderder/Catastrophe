// Fill out your copyright notice in the Description page of Project Settings.


#include "BackPackComponent.h"

#include "GameFramework/Actor.h"

#include "Gameplay/Items/ItemStack.h"
#include "ItemComponent.h"

// Sets default values for this component's properties
UBackPackComponent::UBackPackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBackPackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


class UItemStack* UBackPackComponent::FindItemStack(TSubclassOf<class AActor> _itemClass)
{
	for (UItemStack* stack : BackPack)
	{
		if (stack->ItemClass == _itemClass)
			return stack;
	}

	return nullptr;
}

// Find similar stack, try put the item in
bool UBackPackComponent::InsertItem(class AActor* _itemActor)
{
	if (BackPack.Num() >= MaxBackPackSize)
		return false;

	// Check if actor contains an item component
	UItemComponent* item = 
		Cast<UItemComponent>(
			_itemActor->FindComponentByClass(UItemComponent::StaticClass()));
	if (!item) return false;

	// Insert item to a stack if possible, otherwise create a new stack
	UItemStack* stack = FindItemStack(_itemActor->StaticClass());
	if (stack && stack->StackSize < item->GetItemMaxStackSize())
	{
		stack->StackSize++;
	}
	else
	{
		UItemStack* newStack = NewObject<UItemStack>();
		newStack->ItemClass = _itemActor->StaticClass();
		newStack->StackSize++;
		BackPack.Add(newStack);
	}

	return true;
}

