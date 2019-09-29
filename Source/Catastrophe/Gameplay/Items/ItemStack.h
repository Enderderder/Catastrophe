// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemStack.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_API UItemStack : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, Category = "ItemStack")
	TSubclassOf<class AActor> ItemClass;

	UPROPERTY(BlueprintReadOnly, Category = "ItemStack")
	int32 StackSize = 0;

};
