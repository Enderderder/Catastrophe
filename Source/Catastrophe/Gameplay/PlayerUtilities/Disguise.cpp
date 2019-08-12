
#include "Disguise.h"

#include "Engine/World.h"

#include "Engine/Public/TimerManager.h"

#include "GameFramework/PlayerController.h"
#include "Characters/PlayerCharacter/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/PlayerCharacter/CharacterSprintMovementComponent.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
ADisguise::ADisguise()
{
	PrimaryActorTick.bCanEverTick = false;
	
	DisguiseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisguiseMesh"));
	RootComponent = DisguiseMesh;
	DisguiseMesh->SetVisibility(false);
}

// Called when the game starts or when spawned
void ADisguise::BeginPlay()
{
	Super::BeginPlay();

	
	Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	PlayerMoveSpeed = Player->GetCharacterMovement()->MaxWalkSpeed;

	DisguiseMesh->AttachTo(Player->GetRootComponent());
	DisguiseMesh->RelativeLocation = FVector(0, 0, 10);

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
	Destroy();
}
