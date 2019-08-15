// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSack.generated.h"

UCLASS()
class CATASTROPHE_API AItemSack : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "ItemSack")
	uint8 ItemAmount;
	UPROPERTY(EditDefaultsOnly, Category = "ItemSack")
	uint8 MaxItemAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemSack")
	class UTexture* IconTexture;

public:
	UPROPERTY(EditDefaultsOnly, Category = "ItemSack")
	bool IsAimingNeeded;

public:	
	// Sets default values for this actor's properties
	AItemSack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/**
	 * Called to add on item to the sack
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void AddItem();

	/**
	 * Called to add a certain amount of items to the sack
	 * @author James Johnstone
	 * @param _Amount The Number of items that are to be added to the sack
	 */
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void AddItems(uint8 _Amount);

	/**
	 * Called to fill the item sack up to full
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void FillItemSack();

	/**
	 * Called to check if the item sack is full
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	bool IsItemSackFull() const;

	/**
	 * Called to remove an item from the sack
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void RemoveItem();

	/**
	 * Called to remove a certain amount of items from the sack
	 * @author James Johnstone
	 * @param _Amount The amount of items that are to be removed from the sack
	 */
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void RemoveItems(uint8 _Amount);

	/**
	 * Called to remove all items from the sack
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void EmptyItemSack();

	/**
	 * Called to check if the sack is empty
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	bool IsItemSackEmpty() const;

	/**
	 * Called to set the amount of items that is in the sack
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void SetItemAmount(uint8 _Amount);

	/**
	 * Called to get the amount of items that are currently in the sack
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	uint8 GetItemAmount() const;

	/**
	 * Called to set the max size of the sack
	 * @author James Johnstone
	 * @param _Size The new max size of the sack
	 */
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void SetSackSize(uint8 _Size);

	/**
	 * Called to upgrade the sack size by a certain amount
	 * @author James Johnstone
	 * @param _IncreaseAmount The amount the sack size should be increased by
	 */
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	void UpgradeSackSize(int _IncreaseAmount);

	/**
	 * Called to get the max size of the sack
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	uint8 GetSackSize() const;

	/**
	 * Called to check if an item is able to be used
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "TomatoSack")
	bool IsAbleToUse();

	/** 
	 * Called when the item is to be used
	 * @author James Johnstone
	*/
	virtual void UseItem();
};
