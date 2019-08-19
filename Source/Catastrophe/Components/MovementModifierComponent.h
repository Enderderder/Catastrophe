// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovementModifierComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATASTROPHE_API UMovementModifierComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	/** Origin value record from the UCharacterMovementComponent */
	float CharacterMaxWalkSpeed;
	float CharacterMaxCrouchWalkSpeed;

public:	
	// Sets default values for this component's properties
	UMovementModifierComponent();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float SpeedModifier = 1.0f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	


	void ApplyWalkSpeedModifier(float _modifyValue);

	void ApplyWalkSpeedModifierWithDuration(float _modifyValue, float _duration);


};
