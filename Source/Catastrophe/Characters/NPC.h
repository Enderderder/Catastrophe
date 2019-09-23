
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInteractableComponent* InteractableComponent;

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

	UFUNCTION(BlueprintCallable, Category = "NPC")
	void Interact(class APlayerCharacter* _playerCharacter);
	UFUNCTION(BlueprintImplementableEvent, Category = "NPC", meta = (DisplayName = "OnInteract"))
	void Receive_Interact();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
