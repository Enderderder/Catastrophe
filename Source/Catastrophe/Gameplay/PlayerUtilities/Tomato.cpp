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
	ItemMesh->SetCollisionProfileName(TEXT("Tomato"));
	ItemMesh->OnComponentHit.RemoveDynamic(this, &ATomato::OnTomatoHit);
	ItemMesh->OnComponentHit.AddDynamic(this, &ATomato::OnTomatoHit);
	RootComponent = ItemMesh;
}

// Called when the game starts or when spawned
void ATomato::BeginPlay()
{
	Super::BeginPlay();

	
}

void ATomato::OnTomatoHit(UPrimitiveComponent* _hitComp, AActor* _otherActor, UPrimitiveComponent* _otherComp, FVector _normalImpulse, const FHitResult& _hit)
{
	// If the tomato hits the head of a guard
	if (_otherActor->IsA<AGuard>())
	{
		if (_otherComp->ComponentHasTag(TEXT("Head")))
		{
			AGuard* guard = Cast<AGuard>(_otherActor);
			guard->SetGuardState(EGuardState::STUNED);
		}
	}
	if (!_otherComp->ComponentHasTag("ThrowablesUnaffected"))
	{
		DestroyTomato(_otherActor);
	}
}

void ATomato::DestroyTomato(class AActor* _otherActor)
{
	Receive_OnTomatoSplash(_otherActor);
	Destroy();
}

void ATomato::LaunchTomato(FVector _launchDirection)
{
	ItemMesh->AddForce(_launchDirection * LaunchForce);
}
