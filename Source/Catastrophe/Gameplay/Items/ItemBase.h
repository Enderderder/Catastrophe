// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemTypes.h"
#include "ItemBase.generated.h"

UCLASS()
class CATASTROPHE_API AItemBase : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FItemData ItemInfo;

public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	/**
	 * Virtual function. Called when player uses the item
	 * @author Richard Wulansari
	 * @param _playerCharacter
	 * @note Avoid any useage of self reference
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void UseItem(class APlayerCharacter* _playerCharacter);
	virtual void UseItem_Implementation(class APlayerCharacter* _playerCharacter);


	/** Getter */
	FORCEINLINE FItemData GetItemData() const { return ItemInfo; }
};
