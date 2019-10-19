// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CaveBlocker.generated.h"

UCLASS()
class CATASTROPHE_API ACaveBlocker : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UDestructibleComponent* RockDestructableMesh;

public:	
	// Sets default values for this actor's properties
	ACaveBlocker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
