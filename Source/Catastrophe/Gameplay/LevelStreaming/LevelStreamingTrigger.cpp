// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamingTrigger.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Kismet/GameplayStatics.h"

#include "RespawnSystem/RespawnSubsystem.h"
#include "DebugUtility/CatastropheDebug.h"

// Sets default values
ALevelStreamingTrigger::ALevelStreamingTrigger()
{
 	// This actor should not tick
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->OnComponentBeginOverlap.RemoveDynamic(this, &ALevelStreamingTrigger::OnPlayerEnterTrigger);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelStreamingTrigger::OnPlayerEnterTrigger);
	RootComponent = TriggerBox;

	EditorOnlyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EditorOnlyMesh"));
	EditorOnlyMesh->bIsEditorOnly = true;
	EditorOnlyMesh->bHiddenInGame = true;
	EditorOnlyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EditorOnlyMesh->SetGenerateOverlapEvents(false);
	EditorOnlyMesh->SetupAttachment(TriggerBox);
}

// Called when the game starts or when spawned
void ALevelStreamingTrigger::BeginPlay()
{
	Super::BeginPlay();

}

// Called when trigger collides
void ALevelStreamingTrigger::OnPlayerEnterTrigger(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Player")) && bActive)
	{
		URespawnSubsystem::GetInst(this)->RespawnPlayerAtDistrict(
			DestinationLevelDistrict, RespawnLocationName);
	}
}