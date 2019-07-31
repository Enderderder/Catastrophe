// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterSprintMovementComponent.generated.h"

/** Delegate declaration */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSprintComponentDelegate);

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATASTROPHE_API UCharacterSprintMovementComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	/**  */
	UPROPERTY()
	bool bSprinting = false;

	/**  */
	UPROPERTY()
	bool bWantsToSprint = false;

	/** The walk speed of the player, value will be the same as the CharacterMovementComponent */
	UPROPERTY()
	float WalkSpeed;

	/**  */
	UPROPERTY()
	class UCharacterMovementComponent* CharacterMovementComponent;

	/**  */
	UPROPERTY()
	class ACharacter* CharacterOwner;


public:	
	// Sets default values for this component's properties
	UCharacterSprintMovementComponent();

	/**  */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement | Sprint")
	bool bAllowsToSprint = true;

	/**  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement | Sprint")
	bool bUseConstantSprintSpeed = false;

	/**  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement | Sprint")
	float ConstantSprintSpeed = 700.0f;

	/**  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement | Sprint")
	float SprintSpeedMultiplier = 1.5f;

	/**  */
	UPROPERTY(BlueprintAssignable)
	FSprintComponentDelegate OnSprintBegin;

	/**  */
	UPROPERTY(BlueprintAssignable)
	FSprintComponentDelegate OnSprintEnd;

protected:




protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * 
	 * @author: Richard Wulansari
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement | Sprint")
	void Sprint();

	/**
	 * 
	 * @author: Richard Wulansari
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement | Sprint")
	void UnSprint();




	/** Getter */

	UFUNCTION(BlueprintCallable, Category = "Movement | Sprint")
	bool IsSprinting() const { return bSprinting; }

	/** Getter End */

private:

	/**
	 * Check if the character is able to sprint
	 * @author Richard Wulansari
	 * @return If this component is able to sprint
	 */
	bool IsAbleToSprint() const;

	/**
	 * Checks all the necessary data of this component
	 * @author Richard Wulansari
	 * @return The result of the validation, false means theres some data as null
	 */
	bool HasValidData() const;

};
