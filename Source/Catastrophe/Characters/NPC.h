
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPC.generated.h"

UCLASS()
class CATASTROPHE_API ANPC : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ANPC();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* root;

	// The trigger box which is used to interact
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	// The interaction component that allows the player to interact with the NPC
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInteractableComponent* InteractableComponent;

	// The dialogue system component which allows the player to have a conversation with the NPC
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UDialogueSystemComponent* DialogueSystemComponent;

	// Dialogue settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FString Name;

	UPROPERTY(BlueprintReadWrite, Category = "NPC")
	bool CanNPCTalk;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Called to interact with the NPC
	 * @author James Johnstone
	 * @param _PlayerCharacter The pointer to the player character
	 */
	UFUNCTION(BlueprintCallable, Category = "NPC")
	void Interact(class APlayerCharacter* _PlayerCharacter);
	UFUNCTION(BlueprintImplementableEvent, Category = "NPC", meta = (DisplayName = "OnInteract"))
	void Receive_Interact();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
