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
	class UStaticMeshComponent* ItemMesh;

public:
	// Sets default values for this actor's properties
	ATomato();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called when the mesh component overlapped */
	UFUNCTION()
	void OnTomatoCollisionBeginOverlap(class UPrimitiveComponent* _OverlappedComponent, class AActor* _OtherActor, class UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);

	/** Called to destroy the tomato */
	UFUNCTION()
	void DestroyTomato(class AActor* _otherActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Tomato")
	void Receive_OnTomatoSplash(class AActor* _otherActor);

public:
	/** Launch tomato in certain direction */
	UFUNCTION(BlueprintCallable, Category = "Tomato")
	void LaunchTomato(FVector _launchDirection, float _launchForce);

	/** Getter */
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return ItemMesh; }
	/** Getter End */

};
