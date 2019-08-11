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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CaterpillarAim")
	float Speed;

	/** The in air state of the caterpillar */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CaterpillarAim")
	bool bInAir;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CaterpillarAim")
	class ACaterpillar* CaterpillarCharacter;


protected:
	
	/** Executed when begin play is called on the owning component */
	virtual void NativeBeginPlay() override;

public:

	/** Excuted every tick */
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	/** Getter */
	FORCEINLINE class ACaterpillar* GetCaterpillarCharacter() const { return CaterpillarCharacter; }
	/** Getter End */
};
