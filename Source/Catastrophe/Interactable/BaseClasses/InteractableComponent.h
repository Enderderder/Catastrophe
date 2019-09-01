// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractSingature, class APlayerCharacter*, _playerCharacter);


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

	/** Even called when the player interact with this component */
	UPROPERTY(BlueprintAssignable)
	FInteractSingature OnInteract;

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

	/** Reference to the player character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	class APlayerCharacter* PlayerRef;

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

	/** Called when player interact */
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
	 * 
	 */
	void SetInteractionUiVisible(bool _visibility);
};
