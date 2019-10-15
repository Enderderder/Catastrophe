
#include "DialogueSystemComponent.h"

#include "Blueprint/UserWidget.h"
#include "DialogueSystem/DialogueWidget.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"
#include "Characters/PlayerCharacter/PlayerWidget.h"
#include "Components/BoxComponent.h"
#include "QuestSystem/QuestObjectiveComponent.h"

// Sets default values for this component's properties
UDialogueSystemComponent::UDialogueSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UDialogueSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Reseting all the current conversation and current sentence
	CurrentConversationIndex = 0;
	CurrentSentenceIndex = 0;

	// Initializing dialogue widget
	if (DialogueWidgetRef)
	{
		DialogueWidget = CreateWidget<UDialogueWidget>(GetWorld(), DialogueWidgetRef);
	}

	// Getting the player character and storing it
	Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController());
}

void UDialogueSystemComponent::UpdateBindingToDialogueWidget()
{
	DialogueWidget->BindDialogueComponent(this);
}

void UDialogueSystemComponent::DialogueInteract(class APlayerCharacter* _PlayerCharacter, int _ConversationIndex, bool _bCanInteract)
{
	if (_bCanInteract)
	{
		if (bInConversation)
		{
			if (Conversations[CurrentConversationIndex].Sentences.Num() - 1 <= CurrentSentenceIndex)
			{
				DisableDialogue(true);
				OnSentenceDisappear.Broadcast();
				return;
			}

			CurrentSentenceIndex++;

			UpdateBindingToDialogueWidget();

			// Call the delegate for when a sentence disappears
			OnSentenceChange.Broadcast();
		}
		else
		{
			StartConversation(_ConversationIndex);
			// Call the delegate for when a sentence appears
			OnSentenceAppear.Broadcast();
		}
	}
}

void UDialogueSystemComponent::AttachQuestObjectiveToConversation(int _ConversationIndex, class UQuestObjectiveComponent* _QuestObjectiveComponent)
{
	if (Conversations.Num() > _ConversationIndex)
	{
		Conversations[_ConversationIndex].QuestObjectiveToComplete = _QuestObjectiveComponent;
	}
}

void UDialogueSystemComponent::StartConversation(int _ConversationIndex)
{
	if (Conversations.Num() > _ConversationIndex && DialogueWidget)
	{
		OnConversationStart.Broadcast();

		CurrentConversationIndex = _ConversationIndex;
		CurrentSentenceIndex = 0;
		bInConversation = true;

		if (Player)
		{
			// Sets the player HUD to hidden
			Player->GetPlayerHudWidget()->SetVisibility(ESlateVisibility::Hidden);
		}

		// Adds the dialogue widget to screen
		DialogueWidget->AddToViewport();

		// Calls the function which updates all the dialogue
		UpdateBindingToDialogueWidget();
	}
}

void UDialogueSystemComponent::DisableDialogue(bool _bHasFinishedConversation)
{
	OnDialogueDisable.Broadcast();

	// Remove dialogue widget from the screen
	if (DialogueWidget)
	{
		DialogueWidget->RemoveFromViewport();
	}

	if (Player)
	{
		// Sets the player HUD visible again
		Player->GetPlayerHudWidget()->SetVisibility(ESlateVisibility::Visible);
	}

	if (Conversations.Num() > CurrentConversationIndex && _bHasFinishedConversation)
	{
		OnConversationEnd.Broadcast(CurrentConversationIndex);

		// If an objective has been set, then complete it
		UQuestObjectiveComponent* objective = Conversations[CurrentConversationIndex].QuestObjectiveToComplete;
		if (objective)
		{
			objective->CompleteObjective();
		}
	}

	// Resets current conversation and current sentence indices
	CurrentConversationIndex = 0;
	CurrentSentenceIndex = 0;
	bInConversation = false;
}

FString UDialogueSystemComponent::GetCurrentDialogueText()
{
	if (Conversations.Num() > CurrentConversationIndex)
	{
		FSConversation CurrentConversation = Conversations[CurrentConversationIndex];
		if (CurrentConversation.Sentences.Num() > CurrentSentenceIndex)
		{
			return Conversations[CurrentConversationIndex].Sentences[CurrentSentenceIndex].Text;
		}
	}
	return "";
}

bool UDialogueSystemComponent::IsPlayerTalking()
{
	if (Conversations.Num() > CurrentConversationIndex)
	{
		if (Conversations[CurrentConversationIndex].Sentences.Num() > CurrentSentenceIndex)
		{
			ECharacter CurrentCharacterType = Conversations[CurrentConversationIndex].Sentences[CurrentSentenceIndex].CharacterType;
			return (CurrentCharacterType == ECharacter::ECh_Player) ? true : false;
		}
	}
	return false;
}
