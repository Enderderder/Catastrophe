// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATASTROPHE_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemComponent")
	FString ItemName = "NAME_ITEM_DEFAULT";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemComponent", meta = (MultiLine = "true"))
	FString ItemDescription = "DESCRIPTION_ITEM_DEFAULT";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemComponent")
	class UTexture2D* ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemComponent")
	int32 ItemMaxStackSize = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemComponent")
	bool bUseable = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemComponent")
	bool bCanDiscard = true;

public:	
	// Sets default values for this component's properties
	UItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	
	/** Getter */
	FORCEINLINE FString GetItemName() const { return ItemName; }
	FORCEINLINE FString GetItemDescription() const { return ItemDescription; }
	FORCEINLINE class UTexture2D* GetItemIcon() const { return ItemIcon; }
	FORCEINLINE int32 GetItemMaxStackSize() const { return ItemMaxStackSize; }
	FORCEINLINE bool Useable() const { return bUseable; }
	FORCEINLINE bool CanDiscard() const { return bCanDiscard; }
};
