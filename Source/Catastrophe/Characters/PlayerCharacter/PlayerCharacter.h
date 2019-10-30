// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


/**
 * Player movement sets
 * This determind how the character reacts to the controller inputs at different situations
 */
UENUM(BlueprintType)
enum class EPlayerMovementSet : uint8
{
	NORMAL,
	CAVECHASE
};

/**
 * This struct stores the default value of the player
 */
USTRUCT(BlueprintType)
struct FPlayerDefaultValue
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpVelo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraArmLength;

	FPlayerDefaultValue() :
		WalkSpeed(0.0f),
		JumpVelo(450.0),
		CameraFOV(100.0f),
		CameraArmLength(650.0f)
	{}
};


/**
 * This class is the core of the game. Contains all the information and functionality
 * for the player character
 */
UCLASS()
class CATASTROPHE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	/** Component that modify the movement speed of the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	class UMovementModifierComponent* MovementModifierComponent;
	
	/** Component that controls the sprinting behaviour of the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	class UCharacterSprintMovementComponent* SprintMovementComponent;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Where the camera is going to be focused on normally */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* CamFocusPoint;

	UPROPERTY(BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* ZoomInTimeline;

	/** Where the camera is going to be focused on during aiming */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* AimDownSightFocusPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HHU | General", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BackPackComponent", meta = (AllowPrivateAccess = "true"))
	class UBackPackComponent* BackPackComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HHU | Throwable", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TomatoInHandMesh;

	/** Spawn location for the throwable items */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HHU | Throwable", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ThrowableSpawnPoint;

	// The anchor of the interactable UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* WorldUiAnchor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PostProcess", meta = (AllowPrivateAccess = "true"))
	class UPostProcessComponent* HidingPostProcess;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PostProcess", meta = (AllowPrivateAccess = "true"))
	class UPostProcessComponent* SprintingPostProcess;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PostProcess", meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* SpottedAlertParticle;

protected:

	/** Player animation instance */
	UPROPERTY(BlueprintReadOnly, Category = "Player | Animation")
	class UPlayerAnimInstance* PlayerAnimInstance;

	/** Origin values of the player */
	UPROPERTY(BlueprintReadOnly, Category = "Player | General")
	FPlayerDefaultValue PlayerDefaultValues;
	
	/** The main HUD for the player */
	UPROPERTY(BlueprintReadOnly, Category = "Player | General")
	class UPlayerWidget* PlayerWidget;

	/** The HUD class */
	UPROPERTY(EditDefaultsOnly, Category = "Player | General")
	TSubclassOf<UPlayerWidget> PlayerWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player | General")
	class UQuestWidget* QuestWidget;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate = 45.0f;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate = 45.0f;

	/** Maximum angle the player camera can look up */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float CameraPitchConstrainMax = 60.0f;

	/** Maximum angle the player camera can look down */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float CameraPitchConstrainMin = -60.0f;

	/** The current interactable target */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Interaction")
	class AActor* InteractingTarget;

	/** The target component that player is interacting */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Interaction")
	class UInteractableComponent* InteractingTargetComponent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Interaction")
	bool bInteracting = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Interaction")
	float InteractionTimeHold = 0.0f;

	/** Is HHU(Hand Hold Utility) primary action active */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "HHU | General")
	bool bHHUPrimaryActive = false;

	/** Is HHU(Hand Hold Utility) secondary action active */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "HHU | General")
	bool bHHUSecondaryActive = false;

	/** The HHU zoom curve */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HHU | General")
	class UCurveFloat* ZoomInCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HHU | General")
	float CameraZoomMultiplier;

	/** The obejct types that use during the projectile path prediction */
	UPROPERTY(EditDefaultsOnly, Category = "HHU | Throwable")
	TArray<TEnumAsByte<EObjectTypeQuery>> ThrowablePrecdictObjectType;

	/** Class object that define what object will be throw out as tomato */
	UPROPERTY(EditDefaultsOnly, Category = "HHU | Throwable")
	TSubclassOf<class ATomato> TomatoClass;

	UPROPERTY(EditDefaultsOnly, Category = "HHU | Throwable")
	TSubclassOf<class AThrowableProjectileIndicator> ThrowableProjectilIndicatorClass;

	UPROPERTY(EditDefaultsOnly, Category = "HHU | Throwable")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "HHU | Throwable")
	bool bShowingProjectileIndicator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HHU | Throwable")
	float ThrowingStrength = 2500.0f;

	/** The extra angle when throwing the object, negative number means up */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HHU | Throwable")
	float ThrowingAngle = -30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HHU | Throwable")
	float ThrowableGravityOverwrite = -2000.0f;

	UPROPERTY(BlueprintReadOnly, Category = "HHU | Throwable")
	class AThrowableProjectileIndicator* ThrowableProjectilIndicator;

	UPROPERTY(BlueprintReadOnly, Category = "HHU | Throwable")
	FVector CurrentThrowableLaunchVelocity;

	/** Sprint & Stamina */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float TotalStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float StaminaDrainPerSec = 10.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Movement")
	float CurrentStamina;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Movement")
	bool bSprinting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SpringSpeedMultiplier = 1.3f;

	/** Determine if the player can control the movement of the character */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement")
	bool bAllowMovementInput = true;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Camera")
	bool bAllowCameraInput = true;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement")
	bool bForceCrouch = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "HHU | General")
	bool bCanUseHHU = true;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Interaction")
	bool bCanInteract = true;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Movement")
	EPlayerMovementSet CurrentMovementSet = EPlayerMovementSet::NORMAL;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region Controller Action

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void PlayerJump();

	void PlayerStopJump();

	void PlayerTurn(float _value);

	void PlayerLookUp(float _value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/**
	 * Called when player wants to sprint
	 * @author Richard Wulansari
	 */
	void Sprint();

	/**
	 * Called when player does not want to sprint anymore
	 * @author Richard Wulansari
	 */
	void UnSprint();

	/**
	 * Called when the player character successfully enter sprint action
	 * @author Richard Wulansari
	 */
	UFUNCTION()
	void OnSprintBegin();

	/**
	 * Called when the player character successfully exist sprint action
	 * @author Richard Wulansari
	 */
	UFUNCTION()
	void OnSprintEnd();

	/** Called for character crouching begin */
	void CrouchBegin();

	/** Called for character crouching end */
	void CrouchEnd();

	/** Called when the interacation button pressed */
	void InteractBegin();

	/** Called when the interaction button released */
	void InteractEnd();

	/** Called each tick to do interaction stuff */
	void InteractionTick(float _deltaTime);

	/** HHD(Hand Hold Utility) primary action begin */
	void HHUPrimaryActionBegin();

	/** HHD(Hand Hold Utility) primary action end */
	void HHUPrimaryActionEnd();

	/** HHD(Hand Hold Utility) secondary action begin */
	void HHUSecondaryActionBegin();

	/** HHD(Hand Hold Utility) secondary action end */
	void HHUSecondaryActionEnd();

#pragma endregion Controller Action

	/** Called when ZoomInTimeline ticks */
	UFUNCTION()
	void TimelineSetCameraZoomValue(float _alpha);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * Called to play the grab animation
	 * @author Richard Wulansari
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Player | Animation", meta = (DisplayName = "OnPlayGrabAnim"))
	void Receive_PlayGrabAnim();

	/**
	 * Called to set the T pose state
	 * @author Richard Wulansari
	 * @param _bEnable
	 */
	UFUNCTION(BlueprintCallable, Category = "Player | Animation")
	void SetPlayerTPoseEable(bool _bEnable);

	/**
	 * Set the target to interact for the player
	 * @author Richard Wulansari
	 * @param _interactTargetComponent: The target component where the player will be interact with
	 */
	UFUNCTION()
	void SetInteractionTarget(class UInteractableComponent* _interactTargetComponent);

	/**
	 * Try to remove the interaction target if it exists
	 * @author Richard Wulansari
	 * @param _interactTargetComponent: The interacting component will be removed
	 * @note If the current interacting target component is not the same as the parameter, the current target will not be removed
	 */
	UFUNCTION()
	void RemoveInteractionTarget(class UInteractableComponent* _interactTargetComponent);

	/**
	 * Force the interaction to end from outside of player's control
	 * @author Richard Wulansari
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ResetInteractionAction();

	/**
	 * Set the value of current stamina
	 * @author Richard Wulansari
	 * @note This will not overflow the stamina
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetStamina(float _value);
	
	/**
	 * Called to set if player can control the character movemement related action or not
	 * @author Richard Wulansari
	 * @param _bEnable
	 * @note This will reset the all current action
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMovementActionEnable(bool _bEnable);

	/**
	 * Called to set if player can control the camera rotation
	 * @author Richard Wulansari
	 * @param _bEnable
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetCameraInputEnable(bool _bEnable);

	/**
	 * Called to set if player is forced to crouch
	 * @author Richard Wulansari
	 * @param _bEnable
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetForceCrouchEnable(bool _bEnable);

	/**
	 * Sets the visibility of the player's Hud
	 * @author Richard Wulansari
	 * @param _bEnable
	 */
	UFUNCTION(BlueprintCallable, Category = "Player | General", meta = (DisplayName = "SetPlayerHudEnable"))
	void TogglePlayerHUD(bool _bEnable);

	/**
	 * Toggle the activation state of the spotted alert
	 * This will turn the SpottedParticle on and off
	 * @author Richard Wulansari
	 * @param _bEnable The on/off switch of the spotted particle system component
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void ToggleSpottedAlert(bool _bEnable);

	/** Check if player has tomato in his hand */
	void CheckTomatoInHand();

	/**
	 * Check if the player is crouching
	 * @return True if the player is crouching
	 */
	bool IsPlayerCrouched() const;

	/** Getter */
	FORCEINLINE FPlayerDefaultValue GetPlayerDefaultValues() const { return PlayerDefaultValues; }
	FORCEINLINE class UAIPerceptionStimuliSourceComponent* GetStimulusSourceComponent() const { return PerceptionStimuliSourceComponent; }
	FORCEINLINE float GetTotalStamina() const { return TotalStamina; }
	FORCEINLINE class UPlayerWidget* GetPlayerHudWidget() const { return PlayerWidget; }
	FORCEINLINE class UQuestWidget* GetQuestWidget() const { return QuestWidget; }
	FORCEINLINE class UCameraComponent* GetCamera() const { return FollowCamera; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCharacterSprintMovementComponent* GetSprintMovementComponent() const { 
		return SprintMovementComponent; }
	FORCEINLINE class AThrowableProjectileIndicator* GetProjectileIndicator() const {
		return ThrowableProjectilIndicator;}
	FORCEINLINE class USceneComponent* GetThrowableSpawnPoint() const { return ThrowableSpawnPoint; }
	FORCEINLINE class UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	FORCEINLINE class UBackPackComponent* GetBackPack() const { return BackPackComponent; }
	FORCEINLINE FVector GetCurrentThrowingVelocity() const { return CurrentThrowableLaunchVelocity; }
	FORCEINLINE float GetThrowingGravity() const { return ThrowableGravityOverwrite; }
	FORCEINLINE class UInteractableComponent* GetInteractingTargetComponent() const { return InteractingTargetComponent; }
	/** Getter End */

};
