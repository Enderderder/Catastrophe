// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/PlayerUtilities/UseableItem.h"
#include "Disguise.generated.h"

UCLASS()
class CATASTROPHE_API ADisguise : public AUseableItem
{
	GENERATED_BODY()
	
private:
	/** The amount of seconds the disguise can be used for */
	UPROPERTY(EditDefaultsOnly, Category = "Disguise")
	int SecondsOfUse;

	/** The handle to manage the disguise timer */
	FTimerHandle TimerHandle;

	class APlayerCharacter* Player;

public:
	// Sets default values for this actor's properties
	ADisguise();

protected:
	virtual void BeginPlay() override;

	/**
	 * Called to start the disguise
	 * @author James Johnstone
	 */
	UFUNCTION()
	void UseDisguise();

	/**
	 * Called to end the disguise
	 * @author James Johnstone
	 */
	UFUNCTION()
	void OnEndDisguise();
};
