// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VentilationShortcut.generated.h"

UCLASS()
class CATASTROPHE_API AVentilationShortcut : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ventilation", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ventilation", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PopVentMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ventilation", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* TeleportTransformComponent;

	UPROPERTY(VisibleAnywhere, Category = "Ventilation", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* TeleportArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ventilation", meta = (AllowPrivateAccess = "true"))
	class UInteractableComponent* InteractComponent;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ventilation")
	FString HintText = "This ventilation seems connect to somewhere";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ventilation")
	FString HintTextWhenLocked = "Its locked";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ventilation")
	float VentPopForce = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ventilation")
	float VentPopUpDegree = -30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ventilation")
	bool bLocked = true;

public:	
	// Sets default values for this actor's properties
	AVentilationShortcut();

private:

	/** Interaction component event */
	UFUNCTION()
	void OnPlayerEnterInteractRange(class APlayerCharacter* _playerCharacter);
	UFUNCTION()
	void OnInteractSuccess(class APlayerCharacter* _playerCharacter);
	/** Interaction component event end */

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Called to give player hint
	 * @author Richard Wulansari
	 * @param _playerCharacter
	 * @param _hintText
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Ventilation", meta = (DisplayName = "OnShowHint"))
	void Receive_ShowHint(class APlayerCharacter* _playerCharacter, const FString& _hintText);

public:

	UFUNCTION(BlueprintCallable, Category = "Ventilation")
	void PopVentCap();
	UFUNCTION(BlueprintImplementableEvent, Category = "Ventilation", meta = (DisplayName = "OnPopVentCap"))
	void Receive_PopVentCap();

};
