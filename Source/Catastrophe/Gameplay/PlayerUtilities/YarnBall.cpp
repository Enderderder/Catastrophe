// Fill out your copyright notice in the Description page of Project Settings.


#include "YarnBall.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AYarnBall::AYarnBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	YarnballMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YarnballMesh"));
	YarnballMesh->SetGenerateOverlapEvents(false);
	YarnballMesh->SetCollisionProfileName(TEXT("Yarnball"));
	RootComponent = YarnballMesh;

	// Create stimuli so guards can see yarn ball
	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionStimuliSource"));
}

// Called when the game starts or when spawned
void AYarnBall::BeginPlay()
{
	Super::BeginPlay();
	
}

void AYarnBall::DestroyYarnball()
{
	Receive_OnDestroyYarnball();
	Destroy();
}

void AYarnBall::LaunchYarnball(FVector _launchDirection)
{
	YarnballMesh->AddForce(_launchDirection * LaunchForce);
}
