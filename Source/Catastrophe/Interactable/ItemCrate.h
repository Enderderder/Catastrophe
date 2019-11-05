// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemCrate.generated.h"

UCLASS()
class CATASTROPHE_API AItemCrate : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInteractableComponent* InteractableComponent;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ItemSack")
	TSubclassOf<class AItemSack> ItemSackType;

public:	
	// Sets default values for this actor's properties
	AItemCrate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void PickupItem(class APlayerCharacter* _playerCharacter);
	UFUNCTION(BlueprintImplementableEvent, Category = "ItemCrate", meta = (DisplayName = "OnPickupItem"))
	void Receive_PickupItem();
};
