// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UseableItem.h"
#include "YarnBall.generated.h"

UCLASS()
class CATASTROPHE_API AYarnBall : public AUseableItem
{
	GENERATED_BODY()
	
private:


public:	
	// Sets default values for this actor's properties
	AYarnBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
