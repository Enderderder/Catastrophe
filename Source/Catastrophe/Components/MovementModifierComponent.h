// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovementModifierComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATASTROPHE_API UMovementModifierComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/** Sets default values for this component's properties */
	UMovementModifierComponent();

protected:

	/**
	 * The modifier value that will apply to the character walk speed
	 * @note This value can go down below 0.0f, but during calculation the applied minimum value will always be 0.0f
	 */
	UPROPERTY(VisibleInstanceOnly, Category = "Movement")
	float WalkSpeedModifier = 1.0f;

	/**
	 * The modifier value that will apply to the character crouch movement speed
	 * @note This value can go down below 0.0f, but during calculation the applied minimum value will always be 0.0f
	 */
	UPROPERTY(VisibleInstanceOnly, Category = "Movement")
	float CrouchSpeedModifier = 1.0f;



private:

	/** The movement component this component is updating for */
	UPROPERTY()
	class UCharacterMovementComponent* MovementComponent;

	/** Origin value record from the UCharacterMovementComponent */
	float CharacterMaxWalkSpeed;
	float CharacterMaxCrouchWalkSpeed;

	/** Speed modifier since last change */
	float PreviousWalkSpeedModifier = 1.0f;
	float PreviousCrouchSpeedModifier = 1.0f;



protected:
	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** Called when the outter actor gets destroyed */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	/** Called every frame */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	/**
	 * This will add a new modifier value to all movement speed inside the movement component
	 * @author Richard Wulansari
	 * @param _modifyValue The additional modifier value that will add on to the stack
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ApplySpeedModifierToAll(float _modifyValue);

	/**
	 * This will add a new modifier value to all movement speed inside the movement component
	 * After the duration, the added modify value will be reversed
	 * @author Richard Wulansari
	 * @param _modifyValue The additional modifier value that will add on to the stack
	 * @param _duration The duration of this modifier value lasts
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ApplySpeedModifierWithDurationToAll(float _modifyValue, float _duration);
	
	/**
	 * This will add a new modifier value to max walk speed inside the movement component
	 * @author Richard Wulansari
	 * @param _modifyValue The additional modifier value that will add on to the stack
	 * @see ApplySpeedModifierToAll
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ApplyWalkSpeedModifier(float _modifyValue);

	/**
	 * This will add a new modifier value to max walk speed inside the movement component
	 * After the duration, the added modify value will be reversed
	 * @author Richard Wulansari
	 * @param _modifyValue The additional modifier value that will add on to the stack
	 * @param _duration The duration of this modifier value lasts
	 * @see ApplySpeedModifierWithDurationToAll
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ApplyWalkSpeedModifierWithDuration(float _modifyValue, float _duration);

	/**
	 * This will add a new modifier value to max crouch speed inside the movement component
	 * @author Richard Wulansari
	 * @param _modifyValue The additional modifier value that will add on to the stack
	 * @see ApplySpeedModifierToAll
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ApplyCrouchSpeedModifier(float _modifyValue);

	/**
	 * This will add a new modifier value to max crouch speed inside the movement component
	 * After the duration, the added modify value will be reversed
	 * @author Richard Wulansari
	 * @param _modifyValue The additional modifier value that will add on to the stack
	 * @param _duration The duration of this modifier value lasts
	 * @see ApplySpeedModifierWithDurationToAll
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ApplyCrouchSpeedModifierWithDuration(float _modifyValue, float _duration);


private:

	/**
	 * Validate the data within the component
	 * @author Richard Wulansari
	 * @return The result of the check
	 */
	bool ValidateData();

	/**
	 * Check if any of the modifier value has changed, this usually indicates that speed needs to be updated
	 * @author Richard Wulansari
	 * @return The result of the check
	 */
	bool HasModifierValueChanged();

};
