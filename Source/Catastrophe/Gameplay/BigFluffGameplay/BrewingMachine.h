// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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


	//class 

public:	
	// Sets default values for this actor's properties
	ABrewingMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called when the player interact with the object */
	UFUNCTION()
	void OnInteract(class APlayerCharacter* _playerCharacter);



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
