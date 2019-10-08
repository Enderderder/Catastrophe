// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UseableItem.h"
#include "Tomato.generated.h"

UCLASS()
class CATASTROPHE_API ATomato : public AUseableItem
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TomatoMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

public:
	// Sets default values for this actor's properties
	ATomato();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called when the mesh component hits an object */
	UFUNCTION()
	void OnTomatoHit(UPrimitiveComponent* _hitComp, AActor* _otherActor, UPrimitiveComponent* _otherComp, FVector _normalImpulse, const FHitResult& _hit);

	/**
	 * Called to destroy the tomato
	 * @author James Johnstone
	 * @param _otherActor The other actor which the tomato collided with
	 */
	UFUNCTION()
	void DestroyTomato(class AActor* _otherActor);

	/**
	 * Called to splash tomato
	 * @author James Johnstone
	 * @param _otherActor The actor which the tomato collided with
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Tomato", meta = (DisplayName = "OnTomatoSplash"))
	void Receive_OnTomatoSplash(class AActor* _otherActor);

public:

	/** Getter */
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return TomatoMesh; }
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { 
		return ProjectileMovement; }
	/** Getter End */

};
