// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BrewingMachineAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_API UBrewingMachineAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:



protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnim")
	class ABrewingMachine* BrewingMachineOwner;

protected:
	// Executed when begin play is called on the owning component
	virtual void NativeBeginPlay() override;




public:

	/** Getter */
	FORCEINLINE class ABrewingMachine* GetBrewingMachineOwner() const {
		return BrewingMachineOwner;
	}

	/** Getter End */
};
