// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableUtility.generated.h"

UCLASS()
class CATASTROPHE_API AThrowableUtility : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThrowableUtility", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ThrowableMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThrowableUtility", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

public:	
	// Sets default values for this actor's properties
	AThrowableUtility();

protected:

	/**
	 * Called when the mesh has hit an non "ThrowableAffected" object
	 * @author Richard Wulansari
	 * @param _hitActor: The actor which this throwable has hitted
	 * @param _normalImpulse: The normal vector of the impulse
	 * @param _hit: The hit result
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "ThrowableUtility")
	void OnThrowableHit(AActor* _hitActor, FVector _normalImpulse, const FHitResult& _hit);
	virtual void OnThrowableHit_Implementation(AActor* _hitActor, FVector _normalImpulse, const FHitResult& _hit);

private:

	/**
	 * Called when the mesh has hit something
	 * @author Richard Wulansari
	 */
	UFUNCTION()
	void OnThrowableMeshHit(UPrimitiveComponent* _hitComp, AActor* _otherActor, UPrimitiveComponent* _otherComp, FVector _normalImpulse, const FHitResult& _hit);

public:

	/** Getter */
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return ThrowableMesh; }
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const {
		return ProjectileMovement;
	}
};
