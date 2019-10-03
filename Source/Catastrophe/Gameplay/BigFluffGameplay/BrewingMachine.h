// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gameplay/Items/ItemTypes.h"
#include "BrewingMachine.generated.h"

UCLASS()
class CATASTROPHE_API ABrewingMachine : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BrewingMachine", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* BrewingMachineMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BrewingMachine", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* MeshCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BrewingMachine", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* InteractionTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BrewingMachine", meta = (AllowPrivateAccess = "true"))
	class UInteractableComponent* InteractableComponent;


protected:

	UPROPERTY(BlueprintReadOnly, Category = "BrewingMachine")
	class UBrewingMachineAnimInstance* BrewingMachineAnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BrewingMachine")
	TArray<FRequestItemInfo> RequestCombineItems;

public:	
	// Sets default values for this actor's properties
	ABrewingMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called when the player interact with the object */
	UFUNCTION()
	void OnInteractSuccess(class APlayerCharacter* _playerCharacter);

	UFUNCTION()
	void OnInteractBegin(class APlayerCharacter* _playerCharacter);

	UFUNCTION(BlueprintCallable, Category = "BrewingMachine")
	bool CheckRequiredItems(const TArray<FRequestItemInfo> _requestItems, class UBackPackComponent* _backpack);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:


	

};
