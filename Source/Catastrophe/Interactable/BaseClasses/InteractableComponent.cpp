// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"

#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Characters/PlayerCharacter/PlayerCharacter.h"
#include "Characters/PlayerCharacter/PlayerWidget.h"


// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// This component will tick
	PrimaryComponentTick.bCanEverTick = true;

}

void UInteractableComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (HasValidData() && bShowingUi)
	{
		PlayerHudRef->UpdateInteractionUi(this);

		if (bCanInteract == false)
		{
			SetInteractionUiVisible(false);
		}
	}
}

// When trigger with the player character, set interaction target to it
void UInteractableComponent::OnTriggerWithPlayer(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		if (!PlayerRef)
			PlayerRef = Cast<APlayerCharacter>(OtherActor);

		if (IsValid(PlayerRef))
		{
			PlayerRef->ResetInteractionAction();
			PlayerHudRef = PlayerRef->GetPlayerHudWidget();
			TriggerCounter++;

			OnPlayerEnterInteractRange.Broadcast(PlayerRef);

			if (bCanInteract)
			{
				PlayerRef->SetInteractionTarget(this);
				SetInteractionUiVisible(true);

				// If this component has set to auto interact, interact immediatly
				if (bAutoInteract && IsValid(PlayerRef))
				{
					Interact(PlayerRef, 0.0f);
				}
			}
		}
	}
}

// As player leaves the trigger area, decrease trigger count and if no more trigger count, disable interaction
void UInteractableComponent::OnTriggerEndWithPlayer(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (PlayerRef == OtherActor)
	{
		TriggerCounter--;
		if (TriggerCounter <= 0)
		{
			HoldingTime = 0.0f;
			
			if (PlayerRef->GetInteractingTargetComponent() == this)
			{
				PlayerRef->ResetInteractionAction();
				SetInteractionUiVisible(false);
			}

			bShowingUi = false;
			PlayerRef->RemoveInteractionTarget(this);
			OnPlayerExitInteractRange.Broadcast(PlayerRef);
		}
	}
}

// Called when the player interact with this component
void UInteractableComponent::Interact(class APlayerCharacter* _playerCharacter, float _holdTime)
{
	if (!HasValidData()) return;

	if (bCanInteract)
	{
		HoldingTime = _holdTime;

		// Check if this is the beginning of the interaction
		if (!bInteracting)
		{
			bInteracting = true;
			OnInteractTickBegin.Broadcast(_playerCharacter);
		}

		// Check if player has completed the holding time requirement
		if (_holdTime >= RequiredHoldTime)
		{
			OnInteractSuccess.Broadcast(_playerCharacter);

			// After a successful interaction
			HoldingTime = 0.0f;
			bInteracting = false;
			_playerCharacter->ResetInteractionAction();

			// If the component has set to one time use, disable after interaction
			if (bOneTimeUse)
			{
				bCanInteract = false;
				SetInteractionUiVisible(false);
			}
		}
		else // If not, then broadcast the holding event
		{
			OnInteractTick.Broadcast(_playerCharacter, _holdTime);
		}
	}
}

void UInteractableComponent::StopInteract()
{
	HoldingTime = 0.0f;
}

void UInteractableComponent::RegisterTriggerVolume(class UPrimitiveComponent* _registeringComponent)
{
	// Force the component to generate overlap events
	_registeringComponent->SetGenerateOverlapEvents(true);

	// Binds the function to the component event
	_registeringComponent->OnComponentBeginOverlap.AddDynamic(
		this, &UInteractableComponent::OnTriggerWithPlayer);
	_registeringComponent->OnComponentEndOverlap.AddDynamic(
		this, &UInteractableComponent::OnTriggerEndWithPlayer);
}

// Sets the visibility of the player interaction ui
void UInteractableComponent::SetInteractionUiVisible(bool _visibility)
{
	if (!HasValidData()) return;

	bShowingUi = _visibility;
	PlayerHudRef->UpdateInteractionUi(this);
	PlayerHudRef->SetInteractionUiVisible(_visibility);
}

bool UInteractableComponent::HasValidData()
{
	return 
		IsValid(this) && 
		IsValid(GetOwner()) && 
		IsValid(PlayerRef) &&
		IsValid(PlayerHudRef);
}
