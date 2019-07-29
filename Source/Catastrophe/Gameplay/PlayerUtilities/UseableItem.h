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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ItemMesh;

public:
	// Sets default values for this actor's properties
	AUseableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when using the item
	UFUNCTION(BlueprintCallable)
	virtual void UseAbility();

public:
	/** Getter */
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return ItemMesh; }
	/** Getter End */
};
