// Fill out your copyright notice in the Description page of Project Settings.


#include "Caterpillar.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "Gameplay/GameMode/CatastropheMainGameMode.h"
#include "RespawnSystem/RespawnSubsystem.h"
#include "Gameplay/CaveGameplay/CaterpillarCaveFollowPoint.h"
#include "../PlayerCharacter/PlayerCharacter.h"

#include "DebugUtility/CatastropheDebug.h"

// Sets default values
ACaterpillar::ACaterpillar()
{
 	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 600.0f;

	CatchTriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CatchTriggerMesh"));
	CatchTriggerMesh->SetGenerateOverlapEvents(true);
	CatchTriggerMesh->SetCollisionProfileName(TEXT("Trigger"));
	CatchTriggerMesh->OnComponentBeginOverlap.RemoveDynamic(this, &ACaterpillar::OnCathchPlayerTrigger);
	CatchTriggerMesh->OnComponentBeginOverlap.AddDynamic(this, &ACaterpillar::OnCathchPlayerTrigger);
	CatchTriggerMesh->SetupAttachment(GetMesh());

}

// Called when the game starts or when spawned
void ACaterpillar::BeginPlay()
{
	// Record the original transform
	OriginalTransform = GetActorTransform();

	CatastropheGameMode = Cast<ACatastropheMainGameMode>(
		UGameplayStatics::GetGameMode(this));

	Super::BeginPlay();

	// Search for the cave follow locations and convert them into world location
	TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsOfClass(this, ACaterpillarCaveFollowPoint::StaticClass(), outActors);
	if (outActors.Num() == 1)
	{
		ACaterpillarCaveFollowPoint* caveFollowPointActor = Cast<ACaterpillarCaveFollowPoint>(outActors[0]);
		if (IsValid(caveFollowPointActor))
		{
			TArray<FVector> caveFollowPointsLocalSpace = caveFollowPointActor->GetCaterpillarFollowPoints();
			FTransform caveFollowPointActorTransform = caveFollowPointActor->GetActorTransform();
			for (auto point : caveFollowPointsLocalSpace)
			{
				FVector pointWorldLocation = caveFollowPointActorTransform.TransformPositionNoScale(point);
				CaveFollowPointsWorldSpace.Add(pointWorldLocation);
			}
		}
	}
	else
	{
		CatastropheDebug::OnScreenErrorMsg(TEXT("Insuffient amount of cave follow point, please check world"));
	}
}

void ACaterpillar::OnCathchPlayerTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Reset the cave gameplay as it caught the player
	if (OtherActor->ActorHasTag("Player") && bChaseActive)
	{
		APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OtherActor);
		Receive_OnCatchPlayer(playerCharacter);
	}
}

// Called every frame
void ACaterpillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}