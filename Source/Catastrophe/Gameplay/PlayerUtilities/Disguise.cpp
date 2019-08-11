// Fill out your copyright notice in the Description page of Project Settings.


#include "Disguise.h"
#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
ADisguise::ADisguise()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ADisguise::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	// Start the disguise on spawn
	UseDisguise();
}

void ADisguise::UseDisguise()
{
	if (GetWorld())
	{
		Player->GetStimulusSourceComponent()->UnregisterFromSense(UAISense_Sight::StaticClass());
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ADisguise::OnEndDisguise, SecondsOfUse, false);
	}
}

void ADisguise::OnEndDisguise()
{
	// Make player visible to guards
	Player->GetStimulusSourceComponent()->RegisterForSense(UAISense_Sight::StaticClass());
	Destroy();
}
