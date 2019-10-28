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

	/** The cardboard disguise mesh that spawns over top of the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Disguise", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DisguiseMesh;

	/** The handle to manage the disguise timer */
	FTimerHandle DisguiseTimerHandle;

	/** Stores the player character */
	class APlayerCharacter* Player;

protected:

	/** The amount of seconds the disguise can be used for */
	UPROPERTY(EditDefaultsOnly, Category = "Disguise")
	float DisguiseTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Disguise")
	FVector DisguiseOffset;

public:
	// Sets default values for this actor's properties
	ADisguise();


	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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
