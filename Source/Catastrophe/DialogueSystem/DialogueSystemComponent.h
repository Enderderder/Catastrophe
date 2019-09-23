// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisableDialogueSignature);

UENUM(BlueprintType)
enum class ECharacter : uint8
{
	ECh_NPC		UMETA(DisplayName = "NPC"),
	ECh_Player	UMETA(DisplayName = "Player")
};

USTRUCT(BlueprintType)
struct FSSentence
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	ECharacter CharacterType;

	UPROPERTY(EditAnywhere)
	FString Text;
};

USTRUCT(BlueprintType)
struct FSConversation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FSSentence> Sentences;

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
	UPROPERTY()
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue System")
	class UTexture* NPCIcon;

	// The dialogue widget class type
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue System")
	TSubclassOf<class UUserWidget> DialogueWidgetRef;

	// Stores all the possible conversations which could be had
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue System")
	TArray<FSConversation> Conversations;

public:
	UPROPERTY(BlueprintAssignable, Category = "Dialogue System")
	FOnDisableDialogueSignature OnDialogueDisable;

public:	
	// Sets default values for this component's properties
	UDialogueSystemComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void UpdateDialogue();

	UFUNCTION(BlueprintCallable, Category = "Dialogue System: Quest")
	void AttachQuestObjectiveToConversation(int _ConversationIndex, class UQuestObjectiveComponent* _QuestObjectiveComponent);

public:
	UFUNCTION(BlueprintCallable, Category = "Dialogue System")
	void Interact(class APlayerCharacter* _playerCharacter, int _ConversationIndex);
	
	// Starts the conversation
	UFUNCTION(BlueprintCallable, Category = "Dialogue System")
	void StartConversation(int _ConversationIndex);

	// Stops the conversation
	UFUNCTION(BlueprintCallable, Category = "Dialogue System")
	void DisableDialogue();

	UFUNCTION(BlueprintCallable, Category = "Dialogue System")
	FString GetCurrentDialogueText();

	UFUNCTION(BlueprintCallable, Category = "Dialogue System")
	bool IsPlayerTalking();
};
