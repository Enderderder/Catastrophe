// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThrowableUtility.h"
#include "YarnBall.generated.h"

UCLASS()
class CATASTROPHE_API AYarnBall : public AThrowableUtility
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;

	FTimerHandle LifeTimeHandle;

protected:

	/** Lifetime counts down after the first hit */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YarnBall")
	float Lifetime;

public:	
	// Sets default values for this actor's properties
	AYarnBall();

protected:

	/** AThrowableUtility override */
	virtual void OnThrowableHit_Implementation(AActor* _hitActor, UPrimitiveComponent* _hitComp, FVector _normalImpulse, const FHitResult& _hit) override;
	/** AThrowableUtility override end */


	/**
	 * Blueprint function called when the lifetime of the yarn ball is expired
	 * @author Richard Wulansari
	 * @note The actor will be destroyed right after this function call
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "YarnBall", meta = (DisplayName = "OnYarnBallLifeExpire"))
	void Receive_OnYarnBallLifeExpire();

private:

	/**
	 * Timer function called when the lifetime of the yarn ball is expired
	 * @author Richard Wulansari
	 */
	UFUNCTION()
	void OnYarnBallLifeExpire();
};
