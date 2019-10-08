// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PlayerCharacter/UtilitySacks/ItemSack.h"
#include "ThrowableItemSack.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_API AThrowableItemSack : public AItemSack
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "ItemSack")
	TSubclassOf<class AThrowableUtility> ThrowableUtilityClass;

public:

	/** AItemSack override */
	virtual void UseItem() override;
	/** AItemSack override end */

};
