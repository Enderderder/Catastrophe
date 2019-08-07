// Fill out your copyright notice in the Description page of Project Settings.


#include "Tomato.h"

#include "Components/StaticMeshComponent.h"

#include "Characters/GuardCharacter/Guard.h"

// Sets default values
ATomato::ATomato()
{
 	// Set this actor to not call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

	//TomatoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TomatoMesh"));
	//TomatoMesh->SetGenerateOverlapEvents(true);
	//TomatoMesh->SetCollisionProfileName(TEXT("Throwable"));
	//TomatoMesh->OnComponentBeginOverlap.AddDynamic(this, &ATomato::OnTomatoOverlap);
	//RootComponent = TomatoMesh;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TomatoMesh"));
	ItemMesh->SetGenerateOverlapEvents(true);
	ItemMesh->SetCollisionProfileName(TEXT("Throwable"));
	ItemMesh->OnComponentBeginOverlap.AddDynamic(this, &ATomato::OnItemCollisionBeginOverlap);
	RootComponent = ItemMesh;
}

// Called when the game starts or when spawned
void ATomato::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATomato::OnItemCollision_Implementation(class AActor* _OtherActor, class UPrimitiveComponent* _OtherComp)
{
	// If the tomato hits the head of a guard
	if (_OtherActor->IsA<AGuard>())
	{
		if (_OtherComp->ComponentHasTag(TEXT("Head")))
		{
			AGuard* guard = Cast<AGuard>(_OtherActor);
			guard->SetGuardState(EGuardState::STUNED);
			DestroyTomato(_OtherActor);
		}
	}
	else
	{
		DestroyTomato(_OtherActor);
	}
	// TODO: Spawn decal
}

void ATomato::DestroyTomato(class AActor* _otherActor)
{
	Receive_OnTomatoSplash(_otherActor);
	Destroy();
}

void ATomato::LaunchTomato(FVector _launchDirection, float _launchForce)
{
	//TomatoMesh->AddForce(_launchDirection * _launchForce);
	ItemMesh->AddForce(_launchDirection * _launchForce);
}
