// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEventSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

UGameEventSubsystem::UGameEventSubsystem()
	: UCatastropheGameInstanceSubsystem()
{

}

void UGameEventSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);



}

void UGameEventSubsystem::PostInitialize()
{
	Super::PostInitialize();

}

void UGameEventSubsystem::Deinitialize()
{
	Super::Deinitialize();

}

UGameEventSubsystem* UGameEventSubsystem::GetInst(const UObject* _worldContextObject)
{
	// Try to get the game instance and search for the USaveGameSubsystem instance
	if (UGameInstance * GameInst
		= UGameplayStatics::GetGameInstance(_worldContextObject))
	{
		return GameInst->GetSubsystem<UGameEventSubsystem>();
	}

	return nullptr;
}
