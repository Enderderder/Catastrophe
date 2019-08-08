// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CaterpillarAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_API UCaterpillarAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	/** The movement speed of the caterpillar */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CaterpillarAnim")
	float Speed;


protected:


	class ACaterpillar* CaterpillarCharacter;


};
