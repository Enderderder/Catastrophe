// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThrowableUtility.h"
#include "CatnipBall.generated.h"

UCLASS()
class CATASTROPHE_API ACatnipBall : public AThrowableUtility
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACatnipBall();

protected:

	/** AThrowable override */
	virtual void OnThrowableHit_Implementation(AActor* _hitActor, UPrimitiveComponent* _hitComp, FVector _normalImpulse, const FHitResult& _hit) override;
	virtual void OnThrowableHitEffect_Implementation(AActor* _hitActor, FVector _normalImpulse, const FHitResult& _hit) override;
	/** AThrowable override end */
};
