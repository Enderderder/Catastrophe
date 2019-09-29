// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BackPackComponent.generated.h"

/**
 * This component is a storage for all the collectable item in the game
 * Only for the player character
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATASTROPHE_API UBackPackComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BackPackComponent")
	TArray<class UItemStack*> BackPack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BackPackComponent")
	int32 MaxBackPackSize = 10;


public:	
	// Sets default values for this component's properties
	UBackPackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	 * Called to find an item stack from back pack using an item class
	 * @author Richard Wulansari
	 * @param _itemClass
	 * @return The stack that contains the item in the back pack, if item not in back pack, return null
	 */
	UFUNCTION(BlueprintCallable, Category = "BackPackComponent")
	class UItemStack* FindItemStack(TSubclassOf<class AActor> _itemClass);

public:	
	/**
	 * Called to try put an item into the backpack
	 * @author Richard Wulansari
	 * @param _itemActor: An actor that contains an UItemComponent
	 * @return The result of the action if the item actually gets into the back pack
	 */
	UFUNCTION(BlueprintCallable, Category = "BackPackComponent")
	bool InsertItem(class AActor* _itemActor);

};
