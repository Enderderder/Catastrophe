// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UseableItem.generated.h"

UCLASS()
class CATASTROPHE_API AUseableItem : public AActor
{
	GENERATED_BODY()
	
private:
	

public:
	// Sets default values for this actor's properties
	AUseableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when using the item
	UFUNCTION(BlueprintCallable)
	virtual void UseAbility();

	UFUNCTION()
	void OnItemCollisionBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent)
	void OnItemCollision(class AActor* _OtherActor, class UPrimitiveComponent* OtherComp);
	virtual void OnItemCollision_Implementation(class AActor* _OtherActor, class UPrimitiveComponent* OtherComp);
};
