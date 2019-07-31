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
	TArray<class AItemSack*> SlotsList;

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void ReplaceItemTypeWith(int _Position, TSubclassOf<class AUseableItem> _NewItem);
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void AddItemType(TSubclassOf<class AUseableItem> _NewItem);

	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void ChoosePreviousItem();
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void ChooseNextItem();

	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void UseItem();
};
