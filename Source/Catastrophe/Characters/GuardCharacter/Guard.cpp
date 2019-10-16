// Fill out your copyright notice in the Description page of Project Settings.


#include "Guard.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpotLightComponent.h"
#include "Classes/BehaviorTree/BlackboardComponent.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/PlayerCameraManager.h"

#include "GuardAiController.h"
#include "GuardAnimInstance.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"

#include "Gameplay/GameMode/CatastropheMainGameMode.h"
#include "RespawnSystem/RespawnSubsystem.h"

#include "DebugUtility/CatastropheDebug.h"

// Sets default values
AGuard::AGuard()
{
 	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NavCapsule"));
	GetCapsuleComponent()->CanCharacterStepUpOn = ECB_No;

	HeadHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("HeadHitBox"));
	HeadHitbox->SetGenerateOverlapEvents(true);
	HeadHitbox->SetCollisionProfileName(TEXT("EnemyBlock"));
	HeadHitbox->ComponentTags.Add(TEXT("Head"));
	HeadHitbox->CanCharacterStepUpOn = ECB_No;
	HeadHitbox->SetupAttachment(GetMesh(), TEXT("HeadSocket"));

	HeadShotTargetAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("HeadShotTargetAnchor"));
	HeadShotTargetAnchor->SetupAttachment(GetMesh(), TEXT("HeadSocket"));

	HeadShotTargetMesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("HeadShotTargetMesh"));
	HeadShotTargetMesh->SetGenerateOverlapEvents(false);
	HeadShotTargetMesh->SetCollisionProfileName(TEXT("NoCollision"));
	HeadShotTargetMesh->SetVisibility(false);
	HeadShotTargetMesh->SetupAttachment(HeadShotTargetAnchor);

	BodyHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyHitBox"));
	BodyHitBox->SetGenerateOverlapEvents(true);
	BodyHitBox->SetCollisionProfileName(TEXT("EnemyBlock"));
	BodyHitBox->ComponentTags.Add(TEXT("Body"));
	BodyHitBox->CanCharacterStepUpOn = ECB_No;
	BodyHitBox->SetupAttachment(GetMesh());

	CatchHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CatchHitBox"));
	CatchHitBox->SetGenerateOverlapEvents(true);
	CatchHitBox->SetCollisionProfileName(TEXT("EnemyOverlap"));
	CatchHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); // No collision to begin with
	CatchHitBox->CanCharacterStepUpOn = ECB_No;
	CatchHitBox->OnComponentBeginOverlap.AddDynamic(this, &AGuard::OnCatchHitBoxOverlap);
	CatchHitBox->SetupAttachment(GetMesh());

	// TODO: Pls remove this
	StupidFakeBsHearingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("StupidFakeBsHearingSphere"));
	StupidFakeBsHearingSphere->SetGenerateOverlapEvents(true);
	StupidFakeBsHearingSphere->SetCollisionProfileName(TEXT("EnemyOverlap"));
	StupidFakeBsHearingSphere->CanCharacterStepUpOn = ECB_No;
	StupidFakeBsHearingSphere->SetupAttachment(GetMesh());

	AlertMarkMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AlertMarkMesh"));
	AlertMarkMesh->SetGenerateOverlapEvents(false);
	AlertMarkMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AlertMarkMesh->SetupAttachment(GetMesh());

	QuestionMarkMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("QuestionMarkMesh"));
	QuestionMarkMesh->SetGenerateOverlapEvents(false);
	QuestionMarkMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	QuestionMarkMesh->SetupAttachment(GetMesh());

	ZzzMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ZzzMesh"));
	ZzzMesh->SetGenerateOverlapEvents(false);
	ZzzMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ZzzMesh->SetupAttachment(GetMesh());

	HeadLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("HeadLight"));
	HeadLight->SetRelativeLocation(FVector(16.0f, 0.0f, 0.0f));
	HeadLight->InnerConeAngle = 10.0f;
	HeadLight->OuterConeAngle = 30.0f;
	HeadLight->SetupAttachment(GetMesh(), TEXT("HeadSocket"));
}

// Called when the game starts or when spawned
void AGuard::BeginPlay()
{
	Super::BeginPlay();

	// Sets default values
	DefaultTransform = GetActorTransform();
	
	// Sets the anim instance
	GuardAnimInstance = Cast<UGuardAnimInstance>(GetMesh()->GetAnimInstance());
	if (!GuardAnimInstance)
		UE_LOG(LogTemp, Error, TEXT("Failed to initiate guard anim instance"));

	// Gets reference from the player
	PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!PlayerRef)
		UE_LOG(LogTemp, Error, TEXT("Failed to get player reference"));

	if (ACatastropheMainGameMode* gamemode = ACatastropheMainGameMode::GetGameModeInst(this))
	{
		gamemode->OnPlayerAimingBegin.RemoveDynamic(this, &AGuard::OnPlayerAimingBegin);
		gamemode->OnPlayerAimingBegin.AddDynamic(this, &AGuard::OnPlayerAimingBegin);
		gamemode->OnPlayerAimingEnd.RemoveDynamic(this, &AGuard::OnPlayerAimingEnd);
		gamemode->OnPlayerAimingEnd.AddDynamic(this, &AGuard::OnPlayerAimingEnd);
	}
}

// Called  when actor is detroyed
void AGuard::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ACatastropheMainGameMode* gameMode = ACatastropheMainGameMode::GetGameModeInst(this);
	if (gameMode)
	{
		gameMode->RemoveOneChasingGuard(this);
	}
}

// Called every frame
void AGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate the headshot target plane towards the camera
	// As well as self rotating
	{
		FRotator headShotTargetRot = UKismetMathLibrary::FindLookAtRotation(
			HeadShotTargetAnchor->GetComponentLocation(), 
			UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation());
		HeadShotTargetAnchor->SetWorldRotation(headShotTargetRot);

	}

	// Guard Hearing detection tick
	if (bPlayerInSleepDetectRange)
	{
		if (
			IsValid(PlayerRef) &&
			PlayerRef->GetVelocity().Size() >= 100.0f &&
			!PlayerRef->GetCharacterMovement()->IsCrouching())
		{
			GuardController->GetBlackboardComponent()->SetValueAsBool(TEXT("bHearingPlayer"), true);
		}
		else
		{
			GuardController->GetBlackboardComponent()->SetValueAsBool(TEXT("bHearingPlayer"), false);
		}
	}

}

void AGuard::GetPerceptionLocRot_Implementation(FVector& Location, FRotator& Rotation) const
{
	Location = GetActorLocation() + FVector(0.0f, 0.0f, 80.0f);
	Rotation = GetActorRotation();
}

void AGuard::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	/// Not using parent class implementation

	GetPerceptionLocRot(Location, Rotation);
}

void AGuard::SetGuardState(EGuardState _newState)
{
	// If switching to the same state, ignore it
	if (_newState == GuardState)
		return;

	EGuardState oldState = GuardState;
	GuardState = _newState;

	OnGuardStateChange(oldState, _newState);
}

void AGuard::OnGuardStateChange_Implementation(EGuardState _oldState, EGuardState _newState)
{
	// Refresh the animations of the guard
	StopAllMontages();

	// What will happened when exiting each state
	switch (_oldState)
	{
	case EGuardState::STATIONARY:
		break;
	case EGuardState::SLEEPING:
		ToggleZzzIndicator(false);
		HeadLight->SetVisibility(true);
		if (GuardAnimInstance)
			GuardAnimInstance->bSleeping = false;
		break;
	case EGuardState::WAKEUP_STAGEONE:
		break;
	case EGuardState::WAKEUP_STAGETWO:
		break;
	case EGuardState::PATROLLING:
		break;
	case EGuardState::INVESTATING:
		ToggleQuestionIndicator(false);
		break;
	case EGuardState::CHASING:
		ToggleAlertIndicator(false);
		CatchHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (ACatastropheMainGameMode* gamemode = ACatastropheMainGameMode::GetGameModeInst(this))
			gamemode->RemoveOneChasingGuard(this);
		break;
	case EGuardState::SEARCHING:
		ToggleQuestionIndicator(false);
		break;
	case EGuardState::STUNED:
		break;
	default:
		break;
	}

	// What will happened when entering each state
	switch (_newState)
	{
	case EGuardState::STATIONARY:
		GuardController->ModifySightRange(PatrolSightRange, LosingSightRange);
		break;

	case EGuardState::SLEEPING:
		GuardController->ModifySightRange(0.0f, LosingSightRange);
		ToggleZzzIndicator(true);
		HeadLight->SetVisibility(false);
		if (GuardAnimInstance)
			GuardAnimInstance->bSleeping = true;
		break;

	case EGuardState::WAKEUP_STAGEONE:
		GuardController->ModifySightRange(PatrolSightRange, LosingSightRange);
		break;

	case EGuardState::WAKEUP_STAGETWO:
		break;

	case EGuardState::PATROLLING:
		HeadLight->SetLightColor(NormalHeadLightColor);
		if (bHasPatrolBehaviour && PatrolLocations.Num() > 0)
		{
			SetGuardMaxSpeed(PatrolSpeed);
			GuardController->ModifySightRange(PatrolSightRange, LosingSightRange);
		}
		break;

	case EGuardState::INVESTATING:
		SetGuardMaxSpeed(PatrolSpeed);
		GuardController->ModifySightRange(PatrolSightRange, LosingSightRange);
		ToggleQuestionIndicator(true);
		HeadLight->SetLightColor(NormalHeadLightColor);
		break;

	case EGuardState::CHASING:
		SetGuardMaxSpeed(ChaseSpeed);
		GuardController->ModifySightRange(ChasingSightRange, LosingSightRange);
		ToggleAlertIndicator(true);
		HeadLight->SetLightColor(SpottedHeadLightColor);
		CatchHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		if (ACatastropheMainGameMode* gamemode = ACatastropheMainGameMode::GetGameModeInst(this))
			gamemode->AddChasingGuard(this);
		break;

	case EGuardState::SEARCHING:
		SetGuardMaxSpeed(ChaseSpeed);
		HeadLight->SetLightColor(AlertedHeadLightColor);
		break;

	case EGuardState::STUNED:
		OnStunBegin();
		break;


	default: break;
	}
}

void AGuard::OnStunBegin()
{
	GuardController->GetBlackboardComponent()->SetValueAsBool(
		TEXT("bStunned"), true);

	// Make sure this bi** doesnt move
	GuardController->StopMovement();

	// Sight goes dark for guard
	GuardController->SetGuardSenseEnable_Sight(false);

	if (ACharacter* player = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		GuardController->GetBlackboardComponent()->SetValueAsVector(
			TEXT("PlayerLastSeenLocation"), player->GetActorLocation());
	}

	// Turn off head light
	HeadLight->SetVisibility(false);

	if (GuardAnimInstance)
		GuardAnimInstance->bStuned = true;

	// Clear the old timer
	GetWorld()->GetTimerManager().ClearTimer(StunTimerHnadle);
	// Set a timer to call OnStunEnd()
	GetWorld()->GetTimerManager().SetTimer(
		StunTimerHnadle, this, &AGuard::OnStunEnd, MaxStunTime, false);

	// Call the blueprint implementation
	Receive_OnStunBegin();
}

void AGuard::OnStunEnd()
{
	GuardController->GetBlackboardComponent()->SetValueAsBool(
		TEXT("bStunned"), false);
	
	if (GuardAnimInstance)
		GuardAnimInstance->bStuned = false;

	GuardController->SetGuardSenseEnable_Sight(true);

	// Turn the head light back on
	HeadLight->SetVisibility(true);

	// Call the blueprint implementation
	Receive_OnStunEnd();
}

void AGuard::OnCatchHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If player get caught
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (player)
		{
			OnCatchPlayer(player);
		}
	}
}

void AGuard::SetGuardMaxSpeed(float _speed)
{
	GetCharacterMovement()->MaxWalkSpeed = _speed;
}

void AGuard::StopAllMontages()
{
	if (GuardAnimInstance)
		GuardAnimInstance->StopAllMontages(0.1f);
}

void AGuard::LookAround_Implementation(float& out_montageTime)
{
	/// Blueprint should do the thing
}

void AGuard::OnCatchPlayer_Implementation(APlayerCharacter* _player)
{
	/// Should be implement in derived class
}

void AGuard::ResetGuard()
{
	GetCharacterMovement()->DisableMovement();
	GuardController->ModifySightRange(0.0f);
	GuardController->StopMovement();
	bPlayerWasInSight = false;
	bPlayerInSight = false;
	StopAllMontages();
	//SetActorTransform(DefaultTransform, false, nullptr, ETeleportType::ResetPhysics
	SetActorLocationAndRotation(DefaultTransform.GetLocation(), DefaultTransform.GetRotation());

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AGuard::OnPlayerAimingBegin()
{
	HeadShotTargetMesh->SetVisibility(true);
}

void AGuard::OnPlayerAimingEnd()
{
	HeadShotTargetMesh->SetVisibility(false);
}
