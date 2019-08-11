
#include "TomatoSack.h"

#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Gameplay/PlayerUtilities/Tomato.h"

// Sets default values for this component's properties
ATomatoSack::ATomatoSack()
{
	// No tick component
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts
void ATomatoSack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called when the player is about to throw a tomato
void ATomatoSack::UseItem()
{
	// If the sack is not empty and is able to throw tomatoes
	if (IsAbleToUse())
	{
		// Throw a tomato
		if (GetWorld())
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
				
			// Set the parameter for spawning the tomato
			FVector tomatoSpawnLocation;
			FRotator tomatoSpawnRotation;
			FActorSpawnParameters tomatoSpawnInfo;
			tomatoSpawnInfo.Owner = this;
			tomatoSpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			tomatoSpawnLocation = Player->TomatoSpawnPoint->GetComponentLocation();
			tomatoSpawnRotation = Player->FollowCamera->GetComponentRotation();
			
			// Spawn the tomato
			ATomato* SpawnedTomato = GetWorld()->SpawnActor<ATomato>(TomatoClass, tomatoSpawnLocation, tomatoSpawnRotation, tomatoSpawnInfo);
			if (SpawnedTomato)
			{
				// Adds force to the tomato to make it go flying
				SpawnedTomato->LaunchTomato(Player->FollowCamera->GetForwardVector());
			}

			// Lower the ammo
			RemoveItem();

			// Check if theres tomato left in the hand
			Player->CheckTomatoInHand();
		}
		else
		{
			// Return warning if it cannot find world
			UE_LOG(LogTemp, Warning, TEXT("GetWorld returned NULL"));
		}
	}
}
