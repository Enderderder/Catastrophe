// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CatnipBall.generated.h"

UCLASS()
class CATASTROPHE_API ACatnipBall : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CatnipBallMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

public:	
	// Sets default values for this actor's properties
	ACatnipBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Called when the catnip ball has hit something
	 * @author Richard Wulansari
	 */
	UFUNCTION()
	virtual void OnCatnipBallHit(UPrimitiveComponent* _hitComp, AActor* _otherActor, UPrimitiveComponent* _otherComp, FVector _normalImpulse, const FHitResult& _hit);

public:	

	/** Getter */
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return CatnipBallMesh; }
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const {
		return ProjectileMovement;
	}
	/** Getter End */
};
