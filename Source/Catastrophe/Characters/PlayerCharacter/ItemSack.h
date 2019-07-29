// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemSack.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATASTROPHE_API UItemSack : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	uint8 ItemAmount;

	UPROPERTY(EditDefaultsOnly)
	uint8 MaxItemAmount;

	TSubclassOf<class AUseableItem> SackItemType;

public:	
	// Sets default values for this component's properties
	UItemSack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	/* Change the item type */
	UFUNCTION(BlueprintCallable, Category = "ItemSack")
	void SetItemType(TSubclassOf<class AUseableItem> _NewItemType);

	/* Adds one item into the sack, does not overflow */
	UFUNCTION(BlueprintCallable, Category = "ItemSack")
	void AddItem();

	/* Adds a certain amount of items into the sack, does not overflow */
	UFUNCTION(BlueprintCallable, Category = "ItemSack")
	void AddItems(uint8 _Amount);

	/* Fills the item sack so it is full */
	UFUNCTION(BlueprintCallable, Category = "ItemSack")
	void FillItemSack();

	/* Checks if the item sack is full */
	UFUNCTION(BlueprintCallable, Category = "ItemSack")
	bool IsItemSackFull() const;

	UFUNCTION(BlueprintCallable, Category = "ItemSack")
	void RemoveItem();

	UFUNCTION(BlueprintCallable, Category = "ItemSack")
	void RemoveItems(uint8 _Amount);

	UFUNCTION(BlueprintCallable, Category = "ItemSack")
	void EmptySack();

	UFUNCTION(BlueprintCallable, Category = "ItemSack")
	bool IsSackEmpty() const;

	UFUNCTION(BlueprintCallable, Category = "ItemSack")
	void SetItemAmount(uint8 _Amount);

	UFUNCTION(BlueprintCallable, Category = "ItemSack")
	uint8 GetItemAmount() const;

	UFUNCTION(BlueprintCallable, Category = "ItemSack")
	void SetSackSize(uint8 _Size);

	UFUNCTION(BlueprintCallable, Category = "ItemSack")
	uint8 GetSackSize() const;

	UFUNCTION(BlueprintCallable, Category = "ItemSack")
	bool IsAbleToUse();
};
