// Fill out your copyright notice in the Description page of Project Settings.


#include "QteBobLogicHolder.h"

#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"

#include "QteBobWidget.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"
#include "Characters/PlayerCharacter/PlayerWidget.h"
#include "QteBobWidget.h"


// Sets default values
AQteBobLogicHolder::AQteBobLogicHolder()
{
 	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AQteBobLogicHolder::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind input
	if (GetWorld()->GetFirstPlayerController())
	{
		InputComponent = NewObject<UInputComponent>(this, TEXT("InputComponent"));
		if (InputComponent)
		{
			InputComponent->BindAction("Qte", IE_Pressed, this, &AQteBobLogicHolder::PlayerQteAction);
		}
	}
}

// Called every frame
void AQteBobLogicHolder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if the player ran out of time when the event is happening
	if (EventState == EQteEventState::Pending)
	{
		CurrentEventTime += (DeltaTime / UGameplayStatics::GetGlobalTimeDilation(this));
		if (CurrentEventTime > EventInfo.EventDuration)
		{
			EventState = EQteEventState::FailedByTimeOut;
			OnQteEventComplete.Broadcast(EventState);
			UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
			Destroy();
		}
	}
}

void AQteBobLogicHolder::InitiateEventWithRange(float _range)
{
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (IsValid(playerCharacter))
	{
		QteBobWidget = playerCharacter->GetPlayerHudWidget()->CreateQteBobWidget(this);
		if (QteBobWidget)
		{
			// Make sure the event range is less than 50 and starts after 50
			float range = FMath::Min(_range, 50.0f);
			float start = UKismetMathLibrary::RandomFloatInRange(50.0f, 100.0f - range);
			EventInfo.SuccessAreaRange = range;
			EventInfo.SuccessAreaStart = start;

			UGameplayStatics::SetGlobalTimeDilation(this, TimeDilationDuringEvent);
			QteBobWidget->InitializeQteWidget();

			// Gives a delay before the event actually start
			GetWorld()->GetTimerManager().SetTimer(
				EventActualStartHandle, 
				this, 
				&AQteBobLogicHolder::EventAcutalStart,
				DelayBeforeEventStart * UGameplayStatics::GetGlobalTimeDilation(this),
				false);
		}
	}
}

void AQteBobLogicHolder::InitiateEvent(float _range, float _duration)
{
	EventInfo.EventDuration = _duration;
	InitiateEventWithRange(_range);
}

void AQteBobLogicHolder::PlayerQteAction()
{
	if (EventState == EQteEventState::Pending)
	{
		// Check if the player hits the action within the success range
		if (CurrentEventTime >= (EventInfo.SuccessAreaStart * 0.01f) * EventInfo.EventDuration &&
			CurrentEventTime <= (EventInfo.SuccessAreaStart + EventInfo.SuccessAreaRange) * 0.01f * EventInfo.EventDuration)
		{
			EventState = EQteEventState::Success;
		}
		else
		{
			EventState = EQteEventState::FailedByMissHit;
		}

		// BroadCast the message
		OnQteEventComplete.Broadcast(EventState);

		// Return to normal time and destroy the actor
		UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
		Destroy();
	}
}

void AQteBobLogicHolder::EventAcutalStart()
{
	EventState = EQteEventState::Pending;
	EnableInput(UGameplayStatics::GetPlayerController(this, 0));
}

void AQteBobLogicHolder::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	// Remove the timer at end play
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	QteBobWidget->RemoveFromParent();
}

