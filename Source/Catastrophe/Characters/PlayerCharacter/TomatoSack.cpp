
#include "TomatoSack.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "PlayerCharacter.h"
#include "Gameplay/PlayerUtilities/Tomato.h"
#include "ThrowableProjectileIndicator.h"

#include "DebugUtility/CatastropheDebug.h"

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
	if (IsAbleToUse() &&
		GetWorld())
	{
		APlayerCharacter* playerCharacter = 
			Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
				
		// Set the parameter for spawning the tomato
		FVector spawnLocation = playerCharacter->TomatoSpawnPoint->GetComponentLocation();
		FRotator spawnRotation = playerCharacter->FollowCamera->GetComponentRotation();
		FActorSpawnParameters spawnParam;
		spawnParam.Owner = this;
		spawnParam.SpawnCollisionHandlingOverride = 
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		ATomato* tomato = 
			GetWorld()->SpawnActor<ATomato>(
				TomatoClass, 
				spawnLocation, 
				spawnRotation, 
				spawnParam);
		if (tomato)
		{
			UProjectileMovementComponent* tomatoProjectilMovement = tomato->GetProjectileMovement();
			tomatoProjectilMovement->ProjectileGravityScale =
				playerCharacter->GetThrowingGravity() / GetWorld()->GetGravityZ();
			tomatoProjectilMovement->Velocity = playerCharacter->GetCurrentThrowingVelocity();

			// Lower the ammo
			RemoveItem();

			// Check if theres tomato left in the hand
			playerCharacter->CheckTomatoInHand();
		}
		else
		{
			CatastropheDebug::OnScreenErrorMsg(TEXT("Failed to spawn ATomato"), 10.0f);
		}
	}
	else
	{
		CatastropheDebug::OnScreenErrorMsg(TEXT("Failed to spawn ATomato"), 10.0f);
	}
}
