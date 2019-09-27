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

	/** The new player walk speed for when the player is disguised */
	UPROPERTY(EditDefaultsOnly, Category = "Disguise")
	float DisguiseWalkSpeed;

	/** The cardboard disguise mesh that spawns over top of the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Disguise", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DisguiseMesh;

	/** Stores the original player walk speed */
	float PlayerMoveSpeed;

	/** The handle to manage the disguise timer */
	FTimerHandle TimerHandle;

	/** Stores the player character */
	class APlayerCharacter* Player;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Disguise")
	FVector DisguiseOffset;

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
	UFUNCTION(BlueprintImplementableEvent, Category = "Disguise", meta = (DisplayName = "OnUseDisguise"))
		void Receive_OnUseDisguise();

	/**
	 * Called to end the disguise
	 * @author James Johnstone
	 */
	UFUNCTION()
		void OnEndDisguise();
	UFUNCTION(BlueprintImplementableEvent, Category = "Disguise", meta = (DisplayName = "OnEndDisguise"))
		void Receive_OnEndDisguise();

public:
	/**
	 * Called to get the remaining disguised time
	 * @author James Johnstone
	 */
	UFUNCTION(BlueprintCallable, Category = "Disguise")
	float GetRemainingTime();
};
