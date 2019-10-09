// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSack.h"
#include "DisguiseSack.generated.h"

UCLASS()
class CATASTROPHE_API ADisguiseSack : public AItemSack
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "ItemSack")
	TSubclassOf<class ADisguise> DisguiseClass;

public:
	// Sets default values for this component's properties
	ADisguiseSack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/**
	 * Called to use a disguise
	 * @author James Johnstone
	 */
	virtual void UseItem() override;
};
