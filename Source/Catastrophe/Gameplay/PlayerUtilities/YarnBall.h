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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* YarnballMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Yarnball")
	float LaunchForce;

public:	
	// Sets default values for this actor's properties
	AYarnBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Called to destroy the yarn ball
	 * @author James Johnstone
	 */
	UFUNCTION()
	void DestroyYarnball();
	UFUNCTION(BlueprintImplementableEvent, Category = "Yarnball", meta = (DisplayName = "OnDestroyYarnball"))
	void Receive_OnDestroyYarnball();

public:	
	/**
	 * Called to launch the yarn ball in a certain direction
	 * @author James Johnstone
	 * @param _launchDirection The direction vector in which the yarn ball is to be launched at
	 * @param _launchForce The force at which the yarn ball is to be launched with
	 */
	UFUNCTION(BlueprintCallable, Category = "Yarnball")
	void LaunchYarnball(FVector _launchDirection);

	/** Getter */
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return YarnballMesh; }
	/** Getter End */
};
