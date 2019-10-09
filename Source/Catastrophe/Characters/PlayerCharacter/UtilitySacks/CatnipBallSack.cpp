// Fill out your copyright notice in the Description page of Project Settings.


#include "CatnipBallSack.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "../PlayerCharacter.h"
#include "Gameplay/PlayerUtilities/CatnipBall.h"

#include "DebugUtility/CatastropheDebug.h"

void ACatnipBallSack::UseItem()
{
	// If the sack is not empty and is able to throw tomatoes
	if (IsAbleToUse() &&
		GetWorld())
	{
		APlayerCharacter* playerCharacter =
			Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

		// Set the parameter for spawning the tomato
		FVector spawnLocation = playerCharacter->GetThrowableSpawnPoint()->GetComponentLocation();
		FRotator spawnRotation = playerCharacter->GetCamera()->GetComponentRotation();
		FActorSpawnParameters spawnParam;
		spawnParam.Owner = this;
		spawnParam.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ACatnipBall* catnipBall = 
			GetWorld()->SpawnActor<ACatnipBall>(
				CatnipBallClass,
				spawnLocation,
				spawnRotation,
				spawnParam);
		if (catnipBall)
		{
			UProjectileMovementComponent* projectilMovement = catnipBall->GetProjectileMovement();
			if (projectilMovement)
			{
				projectilMovement->ProjectileGravityScale =
					playerCharacter->GetThrowingGravity() / GetWorld()->GetGravityZ();
				projectilMovement->Velocity = playerCharacter->GetCurrentThrowingVelocity();

				// Lower the ammo
				RemoveItem();
			}
		}
		else
		{
			CatastropheDebug::OnScreenErrorMsg(TEXT("Failed to spawn ACatnipBall"));
		}
	}
}
