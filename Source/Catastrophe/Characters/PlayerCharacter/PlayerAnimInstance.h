// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnim")
	bool bCrouch = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnim")
	bool bInAir = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnim")
	float Speed = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnim")
	float RightSpeed = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnim")
	float ForwardSpeed = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnim")
	bool bAiming = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnim")
	bool bT = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnim")
	bool bDead = false;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerAnim")
	class APlayerCharacter* CustomPlayerCharacter;
	

protected:
	// Executed when begin play is called on the owning component
	virtual void NativeBeginPlay() override;

public:
	// Executed at tick
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "PlayerAnim")
	void ResetAnimationValues();

	/** Custom Getter */
	FORCEINLINE class APlayerCharacter* GetCustomPlayerCharacterOwner() const {
		return CustomPlayerCharacter;
	}
	/** Getter End */
};
