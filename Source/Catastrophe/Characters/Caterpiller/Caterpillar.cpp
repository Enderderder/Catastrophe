// Fill out your copyright notice in the Description page of Project Settings.


#include "Caterpillar.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "Gameplay/CaveGameplay/CaterpillarCaveFollowPoint.h"

#include "DebugUtility/CatastropheDebug.h"

// Sets default values
ACaterpillar::ACaterpillar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACaterpillar::BeginPlay()
{
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
				FVector pointWorldLocation = point;
				caveFollowPointActorTransform.TransformPositionNoScale(pointWorldLocation);
				CaveFollowPointsWorldSpace.Add(pointWorldLocation);
			}
		}
	}
	else
	{
		const FString msg = TEXT("Insuffient amount of cave follow point, please check world");
		CatastropheDebug::OnScreenDebugMsg(-1, 30.0f, FColor::Red, msg);
	}
}

// Called every frame
void ACaterpillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
