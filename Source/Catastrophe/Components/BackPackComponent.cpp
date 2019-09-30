// Fill out your copyright notice in the Description page of Project Settings.


#include "BackPackComponent.h"

#include "GameFramework/Actor.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/InheritableComponentHandler.h"
#include "Engine/SimpleConstructionScript.h"
#include "Engine/SCS_Node.h"

#include "Gameplay/Items/ItemStack.h"
#include "ItemComponent.h"

#include "DebugUtility/CatastropheDebug.h"

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
	UItemStack* stack = FindItemStack(_itemActor->GetClass());
	if (stack && stack->StackSize < item->GetItemMaxStackSize())
	{
		stack->StackSize++;
	}
	else
	{
		UItemStack* newStack = NewObject<UItemStack>();
		newStack->ItemClass = _itemActor->GetClass();
		newStack->StackSize++;

		TArray<UObject*> candidates;
			
		//Blueprint Classes
		{
			UBlueprintGeneratedClass* currentClass =
				Cast<UBlueprintGeneratedClass>(newStack->ItemClass);
			while (currentClass)
			{
				//UInheritableComponentHandler
				if (UInheritableComponentHandler* handler = currentClass->InheritableComponentHandler)
				{
					TArray<UActorComponent*> templates;
					handler->GetAllTemplates(templates);
					for (auto componentTemplate : templates)
					{
						candidates.AddUnique(componentTemplate);
					}
				}

				//USimpleConstructionScript
				if (USimpleConstructionScript* scs = currentClass->SimpleConstructionScript) {
					for (auto node : scs->GetAllNodes())
					{
						candidates.AddUnique(node->ComponentTemplate);
					}
				}

				currentClass = Cast<UBlueprintGeneratedClass>(currentClass->GetSuperClass());
			}
		}
		
		// C++ Classes
		{
			TSubclassOf<AActor> currentClass = newStack->ItemClass;

			while (currentClass)
			{
				TArray<UObject*> currentSubobjects;
				currentClass->GetDefaultObjectSubobjects(currentSubobjects);
				for (auto object : currentSubobjects)
				{
					candidates.AddUnique(object);
				}
				currentClass = currentClass->GetSuperClass();
			}
		}

		FString itemName;
		//Loop through candidate objects until we find the first UItemComponent
		for (auto object : candidates)
		{
			if (auto itemComponent = Cast<UItemComponent>(object))
			{
				itemName = itemComponent->GetItemName();
				break;
			}
		}

		/*AActor* itemActorCDO = newStack->ItemClass->GetDefaultObject<AActor>();
		UItemComponent* item =
			Cast<UItemComponent>(
				itemActorCDO->GetComponentByClass(UItemComponent::StaticClass()));*/

		const FString msg = "Item Name: " + itemName;
		CatastropheDebug::OnScreenDebugMsg(-1, 10.0f, FColor::Green, msg);

		BackPack.Add(newStack);
	}

	return true;
}

