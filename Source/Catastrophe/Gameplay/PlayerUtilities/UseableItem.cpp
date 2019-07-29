// Fill out your copyright notice in the Description page of Project Settings.


#include "UseableItem.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AUseableItem::AUseableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ItemMesh->SetGenerateOverlapEvents(true);
	ItemMesh->SetCollisionProfileName(TEXT("Throwable"));
}

// Called when the game starts or when spawned
void AUseableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUseableItem::UseAbility()
{

}

