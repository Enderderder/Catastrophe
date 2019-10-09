// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableItemSack.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "../PlayerCharacter.h"
#include "Gameplay/PlayerUtilities/ThrowableUtility.h"

#include "DebugUtility/CatastropheDebug.h"


// Called when the player tries to use this utility
void AThrowableItemSack::UseItem()
{
	if (IsAbleToUse())
	{
		APlayerCharacter* playerCharacter =
			Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (IsValid(playerCharacter))
		{
			// Set the parameter for spawning the throwable utility
			FVector spawnLocation = playerCharacter->GetThrowableSpawnPoint()->GetComponentLocation();
			FRotator spawnRotation = playerCharacter->GetCamera()->GetComponentRotation();
			FActorSpawnParameters spawnParam;
			spawnParam.Owner = this;
			spawnParam.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AThrowableUtility* throwableUtility =
				GetWorld()->SpawnActor<AThrowableUtility>(
					ThrowableUtilityClass,
					spawnLocation,
					spawnRotation,
					spawnParam);
			if (throwableUtility)
			{
				UProjectileMovementComponent* projectilMovement = throwableUtility->GetProjectileMovement();
				projectilMovement->ProjectileGravityScale =
					playerCharacter->GetThrowingGravity() / GetWorld()->GetGravityZ();
				projectilMovement->Velocity = playerCharacter->GetCurrentThrowingVelocity();

				RemoveItem();
				playerCharacter->CheckTomatoInHand();
			}
			else
			{
				CatastropheDebug::OnScreenErrorMsg(TEXT("Failed to spawn AThrowableUtility"));
			}
		}
	}
}
