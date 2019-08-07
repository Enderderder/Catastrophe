// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATASTROPHE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// The currently selected item
	UPROPERTY()
	int8 CurrentSelection;

	UPROPERTY()
	TArray<class AItemSack*> SlotsList;

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	 * Called to replace an item with a new item
	 * @author James Johnstone
	 * @param The position in the slot list to replace
	 * @param The new item to replace with
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void ReplaceItemTypeWith(int _Position, class AItemSack* _NewItem);
	/**
	 * Called to add a new item type
	 * @author James Johnstone
	 * @param The new item to be added
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void AddItemType(class AItemSack* _NewItem);

public:
	/**
	 * Called to pick up an item and add it to the 
	 * @author James Johnstone
	 * @param The item sack type to be added
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void PickupItem(TSubclassOf<class AItemSack> _NewItemSack);
	UFUNCTION(BlueprintCallable, Category = "ItemuseSystem")
	void PickupItems(TSubclassOf<class AItemSack> _NewItemSack, int _Amount);

	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	class AItemSack* GetItemSack(int _SlotPosition);
	UFUNCTION(BlueprintCallable,Category = "ItemUseSystem")
	class AItemSack* GetCurrentItemSack();
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	class AItemSack* GetPreviousItemSack();
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	class AItemSack* GetNextItemSack();

	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void ChoosePreviousItem();
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void ChooseNextItem();

	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void UseItem();
};
