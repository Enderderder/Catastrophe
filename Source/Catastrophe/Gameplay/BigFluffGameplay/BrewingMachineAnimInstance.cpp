// Fill out your copyright notice in the Description page of Project Settings.

#include "BrewingMachineAnimInstance.h"

#include "BrewingMachine.h"

#include "DebugUtility/CatastropheDebug.h"

void UBrewingMachineAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	// Store player character reference
	BrewingMachineOwner = Cast<ABrewingMachine>(GetOwningActor());
	if (!BrewingMachineOwner)
	{
		CatastropheDebug::OnScreenErrorMsg(TEXT("BrewingMachineAnimInstance: BrewingMachineOwner is null"));
		UE_LOG(LogTemp, Warning, TEXT("BrewingMachineAnimInstance: BrewingMachineOwner is null"));
	}
}


