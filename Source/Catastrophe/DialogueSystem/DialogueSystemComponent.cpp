
#include "DialogueSystemComponent.h"

#include "Blueprint/UserWidget.h"
#include "DialogueSystem/DialogueWidget.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"
#include "Characters/PlayerCharacter/PlayerWidget.h"
#include "Components/BoxComponent.h"

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

void UDialogueSystemComponent::UpdateDialogue()
{
	FSConversation ActiveConversation = Conversations[CurrentConversationIndex];

	DialogueWidget->BindDialogueComponent(this);

	if (ActiveConversation.Sentences.Num() > CurrentSentenceIndex)
	{
		CurrentSentenceIndex++;
	}
}

void UDialogueSystemComponent::Interact(class APlayerCharacter* _playerCharacter)
{
	if (bInConversation)
	{
		UpdateDialogue();
	}
	else
	{
		StartConversation(CurrentConversationIndex);
	}
}

void UDialogueSystemComponent::StartConversation(int _ConversationIndex)
{
	if (Conversations.Num() > _ConversationIndex && DialogueWidget)
	{
		CurrentConversationIndex = _ConversationIndex;
		CurrentSentenceIndex = 0;
		bInConversation = true;

		if (Player)
		{
			// Sets the player HUD to hidden
			Player->GetPlayerWidget()->SetVisibility(ESlateVisibility::Hidden);
		}

		// Adds the dialogue widget to screen
		DialogueWidget->AddToViewport();

		// Calls the function which updates all the dialogue
		UpdateDialogue();
	}
}

void UDialogueSystemComponent::DisableDialogue()
{
	if (DialogueWidget)
	{
		DialogueWidget->RemoveFromViewport();
	}

	if (Player)
	{
		// Sets the player HUD visible again
		Player->GetPlayerWidget()->SetVisibility(ESlateVisibility::Visible);
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
	ECharacter CurrentCharacterType = Conversations[CurrentConversationIndex].Sentences[CurrentSentenceIndex].CharacterType;
	return (CurrentCharacterType == ECharacter::ECh_Player) ? true : false;
}
