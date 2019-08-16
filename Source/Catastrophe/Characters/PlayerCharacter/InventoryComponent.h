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
	TArray<class AItemSack*> Slots;

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
	UFUNCTION()
	void ReplaceItemTypeWith(int _Position, class AItemSack* _NewItem);

	/**
	 * Called to add a new item type
	 * @author James Johnstone
	 * @param _newItem The new item to be added
	 */
	UFUNCTION()
	void AddItemType(class AItemSack* _NewItem);

public:
	/**
	 * Called to pick up an item and add it to a sack
	 * @author James Johnstone
	 * @param _NewItemSack The item sack type to be added
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void PickupItem(TSubclassOf<class AItemSack> _NewItemSack);

	/**
	 * Called to pick up a set amount of items and add it to a sack
	 * @author James Johnstone
	 * @param _Amount The sack class type
	 * @param _Amount The amount of items that are picked up
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void PickupItems(TSubclassOf<class AItemSack> _NewItemSack, int _Amount);

	/**
	 * Called to check if the inventory contains any item sacks
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	bool IsInventoryEmpty();

	/**
	 * Called to get the number of slots in the inventory
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	int GetNumOfSlots();

	/**
	 * Called to get the item sack at a given position in the list
	 * @author James Johnstone
	 * @param _SlotPosition The position of the sack you want to get
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	class AItemSack* GetItemSack(int _SlotPosition);

	/**
	 * Called to get the item sack of a certain type
	 * @author James Johnstone
	 * @param _SlotPosition The class type of the item sack
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	class AItemSack* GetItemSackOfType(TSubclassOf<class AItemSack> _ItemSackType);

	/**
	 * Called to get the currently selected item sack
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	class AItemSack* GetCurrentItemSack();
	/**
	 * Called to get the previous item sack
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	class AItemSack* GetPreviousItemSack();
	/**
	 * Called to get the next item sack
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	class AItemSack* GetNextItemSack();

	/**
	 * Called to rotate the selection backwards to the previous item
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void ChoosePreviousItem();

	/**
	 * Called to rotate the selection forward to the next item
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void ChooseNextItem();

	/**
	 * Called to use an item from the currently selected item sack
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void UseItem(bool _IsAiming);
};
