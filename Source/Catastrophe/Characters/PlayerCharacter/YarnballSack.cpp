#include "YarnballSack.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Gameplay/PlayerUtilities/YarnBall.h"

// Sets default values for this component's properties
AYarnballSack::AYarnballSack()
{
	// No tick component
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts
void AYarnballSack::BeginPlay()
{
	Super::BeginPlay();

}

void AYarnballSack::UseItem()
{
	if (IsAbleToUse())
	{
		if (GetWorld())
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

			// Set the parameter for spawning yarnball
			FVector yarnballSpawnLocation;
			FRotator yarnballSpawnRotation;
			FActorSpawnParameters yarnballSpawnInfo;
			yarnballSpawnInfo.Owner = this;
			yarnballSpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			yarnballSpawnLocation = Player->GetThrowableSpawnPoint()->GetComponentLocation();
			yarnballSpawnRotation = Player->GetCamera()->GetComponentRotation();

			// Spawn the yarn ball
			AYarnBall* SpawnedYarnball = GetWorld()->SpawnActor<AYarnBall>(YarnballClass, yarnballSpawnLocation, yarnballSpawnRotation, yarnballSpawnInfo);
			if (SpawnedYarnball)
			{
				// Adds force to the yarnball to make it go flying
				SpawnedYarnball->LaunchYarnball(Player->GetCamera()->GetForwardVector());
			}

			// Lower the ammo
			RemoveItem();

			// Check if theres a yarnball left in the hand
			Player->CheckTomatoInHand();
		}
	}
}