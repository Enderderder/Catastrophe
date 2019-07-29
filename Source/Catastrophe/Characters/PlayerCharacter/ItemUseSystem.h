// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemUseSystem.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATASTROPHE_API UItemUseSystem : public UActorComponent
{
	GENERATED_BODY()

private:
	// The currently selected item
	UPROPERTY()
	int8 CurrentSelection;
	// Array of item stuff //

public:	
	// Sets default values for this component's properties
	UItemUseSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void ChoosePreviousItem();
	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void ChooseNextItem();

	UFUNCTION(BlueprintCallable, Category = "ItemUseSystem")
	void UseItem();
};
