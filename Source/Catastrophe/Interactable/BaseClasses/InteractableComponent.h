// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractSingature, class APlayerCharacter*, _playerCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteractTickSingature, class APlayerCharacter*, _playerCharacter, float, _holdingTime);

/**
 * This component control and sends out signal when player is interacting
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATASTROPHE_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableComponent();

	/** Event called when the player enters within the interaction range */
	UPROPERTY(BlueprintAssignable)
	FInteractSingature OnPlayerEnterInteractRange;

	/** Event called when the player exits the interaction range */
	UPROPERTY(BlueprintAssignable)
	FInteractSingature OnPlayerExitInteractRange;

	/** Event called when the player begin to interact with this component */
	UPROPERTY(BlueprintAssignable)
	FInteractSingature OnInteractTickBegin;

	/** Event called when the player interact with this component */
	UPROPERTY(BlueprintAssignable)
	FInteractSingature OnInteractSuccess;

	/** Event called during player interact with this component */
	UPROPERTY(BlueprintAssignable)
	FInteractTickSingature OnInteractTick;

	/** Indicate that if this object can be interacted */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bCanInteract = true;

	/** Internal calculation use, indicates the player interaction holding time on this object */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	float HoldingTime = 0.0f;

	/** The require holding time of the interaction button in order to interact with this object */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	float RequiredHoldTime = 0.0f;

	/** Text that describe the action in order to complete this interaction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FString InteractionActionText = "Press";

	/** Text that describe the result of the interaction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FString InteractionDescriptionText = "Interact";

	/** True if this object can only trigger once, will automatically disable interaction after the initial trigger */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	bool bOneTimeUse = false;

	/** If true, the interaction event will be automatically triggered */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	bool bAutoInteract = false;

protected:

	/** Multi-trigger volume support which keep tracking how many trigger is overlapping the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	int32 TriggerCounter = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Interaction")
	class APlayerCharacter* PlayerRef;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Interaction")
	class UPlayerWidget* PlayerHudRef;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Interaction")
	bool bInteracting = false;

private:

	bool bShowingUi = false;

protected:

	/** Called when registered component overlap event triggers */
	UFUNCTION()
	void OnTriggerWithPlayer(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called when registered component end overlap event triggers*/
	UFUNCTION()
	void OnTriggerEndWithPlayer(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	

	/** Called each frame */
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Called when the player interact with this component
	 * @author Richard Wulasnsari
	 * @param _playerCharacter
	 * @param _holdTime: How long has the player been holding interact
	 */
	void Interact(class APlayerCharacter* _playerCharacter, float _holdTime);

	/**
	 * Register a component that has some trigger volume
	 * @author Richard Wulansari
	 * @param _registeringComponent The component that is receiving the overlap events
	 * @note This function is prefer to be called in constructor
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void RegisterTriggerVolume(class UPrimitiveComponent* _registeringComponent);

	/**
	 * Sets the visiblity of the player interaction Ui
	 * @author Richard Wulansari
	 * @param _visibility
	 */
	void SetInteractionUiVisible(bool _visibility);

private:

	/**
	 * Called to validate the information this component contains
	 * @author Richard Wulansari
	 * @return bool Is data valid
	 */
	bool HasValidData();

};
