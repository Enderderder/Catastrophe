// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.generated.h"

USTRUCT(BlueprintType)
struct FItemInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemComponent", meta = (MultiLine = "true"))
	FString ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemMaxStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanDiscard;

	FItemInfo() :
		ItemName("NAME_ITEM_DEFAULT"),
		ItemDescription("DESCRIPTION_ITEM_DEFAULT"),
		ItemIcon(nullptr),
		ItemMaxStackSize(1),
		bUseable(false),
		bCanDiscard(true)
	{}
};