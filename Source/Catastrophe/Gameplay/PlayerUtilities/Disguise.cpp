
#include "Disguise.h"

#include "Engine/World.h"

#include "Engine/Public/TimerManager.h"

#include "GameFramework/PlayerController.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CharacterSprintMovementComponent.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
ADisguise::ADisguise()
{
	// Setting tick to true
	PrimaryActorTick.bCanEverTick = true;
	
	// Initializing the disguise mesh
	DisguiseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisguiseMesh"));
	RootComponent = DisguiseMesh;
}

// Called when the game starts or when spawned
void ADisguise::BeginPlay()
{
	Super::BeginPlay();

	// Getting the player and storing him as a variable
	Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	// Getting the players original walk speed and storing it
	PlayerMoveSpeed = Player->GetCharacterMovement()->MaxWalkSpeed;

	// Attaching the disguise to the 
	DisguiseMesh->AttachToComponent(Player->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	DisguiseMesh->RelativeLocation = DisguiseOffset;

	// Start the disguise on spawn
	UseDisguise();
}

void ADisguise::UseDisguise()
{
	if (GetWorld())
	{
		Receive_OnUseDisguise();

		// Makes player invisible to the guards
		Player->GetStimulusSourceComponent()->UnregisterFromSense(UAISense_Sight::StaticClass());

		// Makes the player unable to sprint
		Player->GetSprintMovementComponent()->bAllowsToSprint = false;
		Player->GetSprintMovementComponent()->UnSprint();

		// Change player walk speed to disguise walk speed
		Player->GetCharacterMovement()->MaxWalkSpeed = DisguiseWalkSpeed;

		// Starts timer for how long disguise is active for
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ADisguise::OnEndDisguise, SecondsOfUse, false);
	}
}

void ADisguise::OnEndDisguise()
{
	Receive_OnEndDisguise();

	// Make player visible to guards
	Player->GetStimulusSourceComponent()->RegisterForSense(UAISense_Sight::StaticClass());
	// Set the player walk speed to original speed
	Player->GetCharacterMovement()->MaxWalkSpeed = PlayerMoveSpeed;
	// Allow the player to be able to sprint again
	Player->GetSprintMovementComponent()->bAllowsToSprint = true;
	// Deleting the actor from the world
	Destroy();
}

float ADisguise::GetRemainingTime()
{
	// Gets the remaining time from the timer
	if (GetWorld())
	{
		return GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle);
	}
	return 0;
}
