
#include "DisguiseSack.h"

#include "Engine/World.h"
#include "Gameplay/PlayerUtilities/Disguise.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

ADisguiseSack::ADisguiseSack()
{
	// No tick component
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts
void ADisguiseSack::BeginPlay()
{
	Super::BeginPlay();
}

void ADisguiseSack::UseItem()
{
	if (GetWorld())
	{
		// Check if the player is already using a disguise
		TArray<AActor*> FoundDisguises;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), DisguiseClass, FoundDisguises);
		if (FoundDisguises.Num() > 0) return;

		// If the sack is not empty and is able to use disguise
		if (IsAbleToUse())
		{
			FVector disguiseSpawnPoint = FVector(0, 0, 0);
			FRotator disguiseSpawnRotation = FRotator(0, 0, 0);
			FActorSpawnParameters disguiseSpawnInfo;
			disguiseSpawnInfo.Owner = this;
			disguiseSpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			if (GetWorld()->SpawnActor<ADisguise>(DisguiseClass, disguiseSpawnPoint, disguiseSpawnRotation, disguiseSpawnInfo))
			{
				RemoveItem();
			}
		}
	}
}
