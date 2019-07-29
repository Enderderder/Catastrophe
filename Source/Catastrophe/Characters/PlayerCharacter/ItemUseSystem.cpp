// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemUseSystem.h"


// Sets default values for this component's properties
UItemUseSystem::UItemUseSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentSelection = 0;
}

// Called when the game starts
void UItemUseSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

/**
* Called to change the current item selected to the previous item in list
* @author James Johnstone
*/
void UItemUseSystem::ChoosePreviousItem()
{
	//if (CurrentSelection == 0)
	//{
	//	//CurrentSelection = ItemsList.Num() - 1;
	//}
	//else
	//{
	//	CurrentSelection--;
	//}
}

/**
* Called to change the current item selected to the next item in list
* @author James Johnstone
*/
void UItemUseSystem::ChooseNextItem()
{
	/*if (CurrentSelection == ItemsList.Num() - 1)
	{
		CurrentSelection = 0;
	}
	else
	{
		CurrentSelection++;
	}*/
}

/**
* Called to use item
* @author James Johnstone
*/
void UItemUseSystem::UseItem()
{
	
}
