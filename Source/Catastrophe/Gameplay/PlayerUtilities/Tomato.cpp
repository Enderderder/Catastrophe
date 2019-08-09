// Fill out your copyright notice in the Description page of Project Settings.


#include "Tomato.h"

#include "Components/StaticMeshComponent.h"

#include "Characters/GuardCharacter/Guard.h"

// Sets default values
ATomato::ATomato()
{
 	// Set this actor to not call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TomatoMesh"));
	ItemMesh->SetGenerateOverlapEvents(true);
	ItemMesh->SetCollisionProfileName(TEXT("Throwable"));
	ItemMesh->OnComponentBeginOverlap.RemoveDynamic(this, &ATomato::OnTomatoCollisionBeginOverlap);
	ItemMesh->OnComponentBeginOverlap.AddDynamic(this, &ATomato::OnTomatoCollisionBeginOverlap);
	RootComponent = ItemMesh;
}

// Called when the game starts or when spawned
void ATomato::BeginPlay()
{
	Super::BeginPlay();

}

void ATomato::OnTomatoCollisionBeginOverlap(class UPrimitiveComponent* _OverlappedComponent, class AActor* _OtherActor, class UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
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
	ItemMesh->AddForce(_launchDirection * _launchForce);
}
