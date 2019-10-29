// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisableDialogueSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConversationStartSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConversationEndSignature, int, ConversationIndex);

// The types of characters that can talk in a conversation
UENUM(BlueprintType)
enum class ECharacter : uint8
{
	ECh_NPC		UMETA(DisplayName = "NPC"),
	ECh_Player	UMETA(DisplayName = "Player")
};

// A sentence that makes up a conversation
USTRUCT(BlueprintType)
struct FSSentence
{
	GENERATED_BODY()

public:
	// The type of character that is currently talking
	UPROPERTY(EditAnywhere)
	ECharacter CharacterType;

	// The sentence that is spoken
	UPROPERTY(EditAnywhere)
	FString Text;
};

// A conversation which the player and the NPC can have
USTRUCT(BlueprintType)
struct FSConversation
{
	GENERATED_BODY()

public:
	// The ordered array of sentences that make up the conversation
	UPROPERTY(EditAnywhere)
	TArray<FSSentence> Sentences;

	// A pointer to a linked quest objective
	UPROPERTY()
	class UQuestObjectiveComponent* QuestObjectiveToComplete;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATASTROPHE_API UDialogueSystemComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// To check if a conversation is able to be initiated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue System", meta = (AllowPrivateAccess = "true"))
	bool CanInteract;

	// The stored dialogue widget which is presented on screen
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue System", meta = (AllowPrivateAccess = "true"))
	class UDialogueWidget* DialogueWidget;

	// Stores the player character
	class APlayerCharacter* Player;

	// To check if the player is in a conversation
	bool bInConversation;

	// The current conversation number which is active
	int CurrentConversationIndex;

	// The current sentence in the current conversation
	int CurrentSentenceIndex;

protected:
	// The dialogue widget class type
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue System")
	TSubclassOf<class UUserWidget> DialogueWidgetRef;

	// Stores all the possible conversations which could be had
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue System")
	TArray<FSConversation> Conversations;

public:
	// The delegate which contains functionality for when the dialogue is disabled
	UPROPERTY(BlueprintAssignable, Category = "Dialogue System")
	FOnDisableDialogueSignature OnDialogueDisable;

	// The delegate which contains functionality for when the conversation starts
	UPROPERTY(BlueprintAssignable, Category = "Dialogue System")
	FOnConversationStartSignature OnConversationStart;

	// The delegate which contains functionality for when the conversation ends
	UPROPERTY(BlueprintAssignable, Category = "Dialogue System")
	FOnConversationEndSignature OnConversationEnd;

public:	
	// Sets default values for this component's properties
	UDialogueSystemComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	 * Called to update the binding to the dialogue widget
	 * @author James Johnstone
	 */
	void UpdateBindingToDialogueWidget();

	/**
	 * Called to attach a quest objective to a conversation
	 * @author James Johnstone
	 * @param _ConversationIndex The index of what conversation to attach quest objective to
	 * @param _QuestObjectiveComponent The quest objective to attach to the conversation
	 */
	UFUNCTION(BlueprintCallable, Category = "Dialogue System: Quest")
	void AttachQuestObjectiveToConversation(int _ConversationIndex, class UQuestObjectiveComponent* _QuestObjectiveComponent);

public:
	/**
	 * Called to start/continue a conversation
	 * @author James Johnstone
	 * @param _PlayerCharacter The player character reference
	 * @param _ConversationIndex The index of the conversation that is to be called
	 */
	UFUNCTION(BlueprintCallable, Category = "Dialogue System")
	void DialogueInteract(class APlayerCharacter* _PlayerCharacter, int _ConversationIndex, bool _bCanInteract);
	
	/**
	 * Called to start a new conversation
	 * @author James Johnstone
	 * @param _ConversationIndex The index of the conversation that is to be started
	 */
	void StartConversation(int _ConversationIndex);

	/**
	 * Called to disable a conversation
	 * @author James Johnstone
	 * @param _bHasFinishedConversation Determines if the conversation has been finished or is disabling in middle of conversation
	 */
	UFUNCTION(BlueprintCallable, Category = "Dialogue System")
	void DisableDialogue(bool _bHasFinishedConversation);

	/**
	 * Called to get the current dialogue sentence text
	 * @author James Johnstone
	 * @returns FString
	 */
	UFUNCTION(BlueprintCallable, Category = "Dialogue System")
	FString GetCurrentDialogueText();

	/**
	 * Called to check if the player is talking
	 * @author James Johnstone
	 * @returns bool
	 */
	UFUNCTION(BlueprintCallable, Category = "Dialogue System")
	bool IsPlayerTalking();
};
