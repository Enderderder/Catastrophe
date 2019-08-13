// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
//#include "Camera/CameraActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/PostProcessComponent.h"
#include "Classes/Particles/ParticleSystemComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Hearing.h"

#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"

#include "Gameplay/GameMode/CatastropheMainGameMode.h"
#include "PlayerWidget.h"
#include "PlayerAnimInstance.h"
#include "CharacterSprintMovementComponent.h"
#include "Interactable/InteractActor.h" /// TODO: Remove this
#include "Interactable/BaseClasses/InteractableObject.h" /// TODO: Remove this
#include "Interactable/BaseClasses/InteractableComponent.h"
#include "Gameplay/PlayerUtilities/Tomato.h"
#include "Gameplay/CaveGameplay/CaveCameraTrack.h"

#include "InventoryComponent.h"
#include "TomatoSack.h"

#include "DebugUtility/CatastropheDebug.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame. 
	PrimaryActorTick.bCanEverTick = true;

	// Set up the sprint movement component
	SprintMovementComponent = CreateDefaultSubobject<UCharacterSprintMovementComponent>(TEXT("SprintMovementComponent"));
	SprintMovementComponent->bUseConstantSprintSpeed = false;
	SprintMovementComponent->OnSprintBegin.RemoveDynamic(this, &APlayerCharacter::OnSprintBegin);
	SprintMovementComponent->OnSprintBegin.AddDynamic(this, &APlayerCharacter::OnSprintBegin);
	SprintMovementComponent->OnSprintEnd.RemoveDynamic(this, &APlayerCharacter::OnSprintEnd);
	SprintMovementComponent->OnSprintEnd.AddDynamic(this, &APlayerCharacter::OnSprintEnd);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	AimDownSightFocusPoint = CreateDefaultSubobject<USceneComponent>(TEXT("AimDownSightFocusPoint"));
	AimDownSightFocusPoint->SetupAttachment(GetMesh());

	CamFocusPoint = CreateDefaultSubobject<USceneComponent>(TEXT("FollowCameraFocusPoint"));
	CamFocusPoint->SetupAttachment(RootComponent);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(CamFocusPoint);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	TomatoSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TomatoSpawnPoint"));
	TomatoSpawnPoint->SetupAttachment(GetMesh());

	// Set the tomato that will show inside players hand
	TomatoInHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TomatoInHandMesh"));
	TomatoInHandMesh->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));

	// Creates an item inventory component which stores useable items
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	WorldUiAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("InteractableWidgetAnchor"));
	WorldUiAnchor->SetupAttachment(RootComponent);

	InteractableUiComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractableUiComponent"));
	InteractableUiComponent->bVisible = false;
	InteractableUiComponent->SetupAttachment(WorldUiAnchor);
	
	// Create stimuli
	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionStimuliSource"));
	
	// Hiding post-process
	HidingPostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("HidingPostProcess"));
	HidingPostProcess->SetupAttachment(RootComponent);

	// Sprinting post-process
	SprintingPostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("SprintingPostProcess"));
	SprintingPostProcess->SetupAttachment(RootComponent);

	SpottedAlertParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SpottedAlertParticle"));
	SpottedAlertParticle->SetupAttachment(GetMesh());
	SpottedAlertParticle->SetVisibility(false);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Configure player camera
	UGameplayStatics::GetPlayerCameraManager(this, 0)->ViewPitchMin = CameraPitchConstrainMin;
	UGameplayStatics::GetPlayerCameraManager(this, 0)->ViewPitchMax = CameraPitchConstrainMax;

	// Gets the player animation instance
	PlayerAnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (!PlayerAnimInstance) UE_LOG(LogTemp, Error, TEXT("Player is not using the correct anim instance"));

	// Construct the zoom in timeline
	if (!ZoomInCurve) UE_LOG(LogTemp, Error, TEXT("Player zoom in curve is nullptr!"));
	ZoomInTimeline = NewObject<UTimelineComponent>(this, TEXT("ZoomInTimeline"));
	ZoomInTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	this->BlueprintCreatedComponents.Add(ZoomInTimeline);
	ZoomInTimeline->SetLooping(false);
	ZoomInTimeline->SetTimelineLength(0.3f);
	ZoomInTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
	FOnTimelineFloat onTimelineCallback;
	onTimelineCallback.BindUFunction(this, FName{ TEXT("TimelineSetCameraZoomValue") });
	ZoomInTimeline->AddInterpFloat(ZoomInCurve, onTimelineCallback);
	ZoomInTimeline->RegisterComponent();

	// Set the stamina to full
	SetStamina(TotalStamina);

	// Store the default values
	PlayerDefaultValues.WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	PlayerDefaultValues.CameraFOV = FollowCamera->FieldOfView;
	PlayerDefaultValues.CameraArmLength = CameraBoom->TargetArmLength;

	// Check if theres tomato in player's hand
	CheckTomatoInHand();

	// Set default state for the player UI
	if (PlayerWidgetClass)
	{
		PlayerWidget = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);
		if (PlayerWidget)
		{
			PlayerWidget->AddToViewport();

			PlayerWidget->ToggleStamina(true);
			PlayerWidget->ToggleCrosshair(false);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Player hud widget class, failed to initiate player widget"));
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If player is sprinting, drain it XD
	if (SprintMovementComponent->IsSprinting())
	{
		CurrentStamina = FMath::Max(0.0f, CurrentStamina - (StaminaDrainPerSec * DeltaTime));
		if (CurrentStamina <= 0.0f) 
			UnSprint();
	}
	else
	{
		CurrentStamina = FMath::Min(TotalStamina, CurrentStamina + (StaminaDrainPerSec * DeltaTime));
	}

	// Make sure to run the zoom in timeline
	if (ZoomInTimeline)
	{
		ZoomInTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}

	// Do the interaction tick
	InteractionTick(DeltaTime);

	// Need to get the ui always face the camera
	//FRotator uiRot = (FollowCamera->GetComponentLocation() - WorldUiAnchor->GetComponentLocation()).Rotation();
	//WorldUiAnchor->SetWorldRotation(uiRot);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// General movement input setup
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::UnSprint);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::CrouchBegin);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::CrouchEnd);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

	// Interaction actions
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::InteractBegin);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APlayerCharacter::InteractEnd);

	// HHU (Hand Hold utility) actions
	PlayerInputComponent->BindAction("HHUPrimary", IE_Pressed, this, &APlayerCharacter::HHUPrimaryActionBegin);
	PlayerInputComponent->BindAction("HHUPrimary", IE_Released, this, &APlayerCharacter::HHUPrimaryActionEnd);
	PlayerInputComponent->BindAction("HHUSecondary", IE_Pressed, this, &APlayerCharacter::HHUSecondaryActionBegin);
	PlayerInputComponent->BindAction("HHUSecondary", IE_Released, this, &APlayerCharacter::HHUSecondaryActionEnd);
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// Calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Sprint()
{
	if (bAllowMovementInput
		&& CurrentStamina >= TotalStamina // Only sprint player has stamina
		&& !bHHUSecondaryActive) // Cant sprint while aiming lol
	{
		SprintMovementComponent->Sprint();
	}
// 	{
// 		bSprinting = true;
// 		SprintingPostProcess->bEnabled = true;
// 
// 		FollowCamera->SetFieldOfView(PlayerDefaultValues.CameraFOV + 5.0f);
// 		GetCharacterMovement()->MaxWalkSpeed = PlayerDefaultValues.WalkSpeed * SpringSpeedMultiplier;
// 	}
}

void APlayerCharacter::UnSprint()
{
	SprintMovementComponent->UnSprint();
// 	if (bSprinting)
// 	{
// 		FollowCamera->SetFieldOfView(PlayerDefaultValues.CameraFOV);
// 		GetCharacterMovement()->MaxWalkSpeed = PlayerDefaultValues.WalkSpeed;
// 		SprintingPostProcess->bEnabled = false;
// 	}
// 	bSprinting = false;
}

void APlayerCharacter::OnSprintBegin()
{
	FollowCamera->SetFieldOfView(PlayerDefaultValues.CameraFOV + 5.0f);
	SprintingPostProcess->bEnabled = true;
}

void APlayerCharacter::OnSprintEnd()
{
	FollowCamera->SetFieldOfView(PlayerDefaultValues.CameraFOV);
	SprintingPostProcess->bEnabled = false;
}

void APlayerCharacter::CrouchBegin()
{
	if (bAllowMovementInput)
	{
		Crouch();
		HidingPostProcess->bEnabled = true;

		HHUSecondaryActionEnd();
		if (SprintMovementComponent->IsSprinting())
			SprintMovementComponent->UnSprint();
	}
}

void APlayerCharacter::CrouchEnd()
{
	UnCrouch();
	HidingPostProcess->bEnabled = false;
}

void APlayerCharacter::CheckTomatoInHand()
{
	if (InventoryComponent)
	{
		if (InventoryComponent->GetCurrentItemSack())
		{
			if (InventoryComponent->GetCurrentItemSack()->IsAbleToUse())
			{
				TomatoInHandMesh->SetVisibility(true);
				return;
			}
		}
	}

	TomatoInHandMesh->SetVisibility(false);
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Controller
		&& (Value != 0.0f) 
		&& bAllowMovementInput)
	{
		FRotator Rotation;
		FRotator YawRotation;

		// find out which way is forward
		switch (CurrentMovementSet)
		{
		case EPlayerMovementSet::NORMAL:
		{
			Rotation = Controller->GetControlRotation();
			YawRotation = FRotator(0, Rotation.Yaw, 0);
			break;
		}
		case EPlayerMovementSet::CAVECHASE:
		{
			if (ACatastropheMainGameMode* mainGameMode = ACatastropheMainGameMode::GetGameModeInst(this))
			{
				if (ACaveCameraTrack* caveCameraTrack = mainGameMode->GetCaveCameraTrack())
				{
					FVector trackCameraLocation = caveCameraTrack->GetTrackFollowCamera()->GetComponentLocation();
					Rotation = UKismetMathLibrary::FindLookAtRotation(trackCameraLocation, GetActorLocation());
					YawRotation = FRotator(0, Rotation.Yaw, 0);
				}
			}
			break;
		}
		default: break;
		}

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Controller 
		&& (Value != 0.0f) 
		&& bAllowMovementInput)
	{
		FRotator Rotation;
		FRotator YawRotation;
		// find out which way is forward
		switch (CurrentMovementSet)
		{
		case EPlayerMovementSet::NORMAL:
		{
			Rotation = Controller->GetControlRotation();
			YawRotation = FRotator(0, Rotation.Yaw, 0);
			break;
		}
		case EPlayerMovementSet::CAVECHASE:
		{
			if (ACatastropheMainGameMode* mainGameMode = ACatastropheMainGameMode::GetGameModeInst(this))
			{
				if (ACaveCameraTrack* caveCameraTrack = mainGameMode->GetCaveCameraTrack())
				{
					FVector trackCameraLocation = caveCameraTrack->GetTrackFollowCamera()->GetComponentLocation();
					Rotation = UKismetMathLibrary::FindLookAtRotation(trackCameraLocation, GetActorLocation());
					YawRotation = FRotator(0, Rotation.Yaw, 0);
				}
			}
			break;
		}
		default: break;
		}

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::TimelineSetCameraZoomValue(float _alpha)
{
	float resultLength =
		FMath::Lerp(
			PlayerDefaultValues.CameraArmLength, 
			PlayerDefaultValues.CameraArmLength * CameraZoomMultiplier, _alpha);
	CameraBoom->TargetArmLength = resultLength;
}

void APlayerCharacter::InteractBegin()
{
	if (IsValid(InteractingTargetComponent))
	{
		bInteracting = true; // Set the holding interaction begin
		InteractingTargetComponent->Interact(this, InteractionTimeHold);
	}
}

void APlayerCharacter::InteractEnd()
{
	bInteracting = false;
	InteractionTimeHold = 0.0f;
}

void APlayerCharacter::InteractionTick(float _deltaTime)
{
	if (bInteracting
		&& IsValid(InteractingTargetComponent))
	{
		InteractionTimeHold += _deltaTime;
		InteractingTargetComponent->Interact(this, InteractionTimeHold);

		// Debug message
		const FString msg = "Interaction hold: " + FString::SanitizeFloat(InteractionTimeHold);
		CatastropheDebug::OnScreenDebugMsg(-1, 0.0f, FColor::Green, msg);
	}
}

void APlayerCharacter::HHUPrimaryActionBegin()
{
	// If cannot use HHU, just don't then
	if (!bCanUseHHU) return;

	// Set the activation state to true
	bHHUPrimaryActive = true;

	// If the player is aiming and shoots, then use currently selected useable item
	if (bHHUSecondaryActive)
	{
		InventoryComponent->UseItem();
	}
}

void APlayerCharacter::HHUPrimaryActionEnd()
{
	// Only end if the action is already activated
	if (bHHUPrimaryActive)
	{
		switch (ActiveHHUType)
		{
		case EHHUType::TOMATO:
			break;
		case EHHUType::LASER:
			break;

		default: break;
		}

		// Flip the activation state
		bHHUPrimaryActive = false;
	}
}

void APlayerCharacter::HHUSecondaryActionBegin()
{
	// If cannot use HHU, just don't then
	if (!bCanUseHHU) return;

	// Set the activation state to true
	bHHUSecondaryActive = true;

	switch (ActiveHHUType)
	{
	case EHHUType::TOMATO:
	{
		// Let the character follow camera rotation
		if (SprintMovementComponent->IsSprinting())
			UnSprint();
		bUseControllerRotationYaw = true;
		CameraBoom->bEnableCameraLag = false;
		CameraBoom->AttachToComponent(
			AimDownSightFocusPoint, FAttachmentTransformRules::KeepRelativeTransform);
		if (ZoomInTimeline)
			ZoomInTimeline->Play();
		PlayerAnimInstance->bAiming = true;
		if (PlayerWidget) PlayerWidget->ToggleCrosshair(true);
		break;
	}

	case EHHUType::LASER:
	{

		break;
	}

	default: break;
	}
}

void APlayerCharacter::HHUSecondaryActionEnd()
{
	// Only end if the action is already activated
	if (bHHUSecondaryActive)
	{
		switch (ActiveHHUType)
		{
		case EHHUType::TOMATO:
		{
			// Let the character not follow camera rotation
			bUseControllerRotationYaw = false;
			CameraBoom->bEnableCameraLag = true;
			CameraBoom->AttachToComponent(
				CamFocusPoint, FAttachmentTransformRules::KeepRelativeTransform);
			if (ZoomInTimeline)
				ZoomInTimeline->Reverse();
			PlayerAnimInstance->bAiming = false;
			PlayerWidget->ToggleCrosshair(false);
			break;
		}

		case EHHUType::LASER:
		{

			break;
		}

		default: break;
		}

		// Flip the activation state
		bHHUSecondaryActive = false;
	}
}

UInventoryComponent* APlayerCharacter::GetInventoryComponent()
{
	return InventoryComponent;
}

void APlayerCharacter::SetInteractionTarget(class UInteractableComponent* _interactTargetComponent)
{
	if (IsValid(_interactTargetComponent))
	{
		InteractingTargetComponent = _interactTargetComponent;
	}
}

void APlayerCharacter::RemoveInteractionTarget(class UInteractableComponent* _interactTargetComponent)
{
	if (_interactTargetComponent == InteractingTargetComponent)
	{
		InteractingTargetComponent = nullptr;
	}
}

void APlayerCharacter::ResetInteractionAction()
{
	InteractEnd();
}

void APlayerCharacter::SetStamina(float _value)
{
	CurrentStamina = FMath::Min(_value, TotalStamina);
}

void APlayerCharacter::BlockMovementAction(bool _bBlockMovementInput)
{
	// Reset the sprint and crouch state
	UnSprint();
	//ForceSprintEnd();
	CrouchEnd();

	// If choose to block input as well
	if (_bBlockMovementInput)
	{
		bAllowMovementInput = false;
	}
}

void APlayerCharacter::UnblockMovementInput()
{
	bAllowMovementInput = true;
}

void APlayerCharacter::ForceUnSprint()
{
	UnSprint();
	FollowCamera->SetFieldOfView(PlayerDefaultValues.CameraFOV);
	GetCharacterMovement()->MaxWalkSpeed = PlayerDefaultValues.WalkSpeed;
}

void APlayerCharacter::TogglePlayerHUD(bool _bEnable)
{
	if (PlayerWidget)
	{
		if (_bEnable)
		{
			PlayerWidget->RemoveFromParent();
			PlayerWidget->AddToViewport(0);
		}
		else
		{
			PlayerWidget->RemoveFromParent();
		}
	}
}

void APlayerCharacter::ToggleInteractUI(bool _bEnable)
{
	InteractableUiComponent->SetVisibility(_bEnable);
}

void APlayerCharacter::ToggleSpottedAlert(bool _bEnable)
{
	if (_bEnable)
	{
		SpottedAlertParticle->ActivateSystem();
	}
	else
	{
		SpottedAlertParticle->DeactivateSystem();
	}
}

bool APlayerCharacter::IsPlayerCrouched() const
{
	return GetCharacterMovement()->IsCrouching();
}

