// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSack.h"
#include "CatnipBallSack.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_API ACatnipBallSack : public AItemSack
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "ItemSack")
	TSubclassOf<class ACatnipBall> CatnipBallClass;

public:
	virtual void UseItem() override;

};
