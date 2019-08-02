// Fill out your copyright notice in the Description page of Project Settings.


#include "UseableItem.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AUseableItem::AUseableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetGenerateOverlapEvents(true);
	ItemMesh->SetCollisionProfileName(TEXT("Throwable"));
	ItemMesh->OnComponentBeginOverlap.RemoveDynamic(this, &AUseableItem::OnItemCollisionBeginOverlap);
	ItemMesh->OnComponentBeginOverlap.AddDynamic(this, &AUseableItem::OnItemCollisionBeginOverlap);
	RootComponent = ItemMesh;
}

// Called when the game starts or when spawned
void AUseableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUseableItem::UseAbility()
{

}

void AUseableItem::OnItemCollisionBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnItemCollision(OtherActor, OtherComp);
}

void AUseableItem::OnItemCollision_Implementation(class AActor* _OtherActor, class UPrimitiveComponent* OtherComp)
{

}

