// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Items/ItemBase.h"
#include "CollectableItem.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHE_API ACollectableItem : public AItemBase
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DefaultRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInteractableComponent* InteractableComponent;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "CollectableItem")
	bool bDestroyImmediateAfterCollect = true;

public:
	// Default constructor
	ACollectableItem();

private:

	UFUNCTION()
	void OnInteractSuccess(class APlayerCharacter* _playerCharacter);

protected:

	/**
	 * 
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "CollectableItem")
	void AfterCollectedEffect(class APlayerCharacter* _playerCharacter);
	virtual void AfterCollectedEffect_Implementation(class APlayerCharacter* _playerCharacter);

};
