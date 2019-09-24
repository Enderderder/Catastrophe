// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableProjectileIndicator.generated.h"

UCLASS()
class CATASTROPHE_API AThrowableProjectileIndicator : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectileIndicator", meta = (AllowPrivateAccess = "true"))
	class USplineComponent* SplineComponent;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectileIndicator")
	int32 NumberOfMeshSegments = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectileIndicator")
	class UStaticMesh* IndicatorMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectileIndicator")
	class UMaterialInterface* IndicatorMaterial;

	UPROPERTY(BlueprintReadWrite)
	TArray<class USplineMeshComponent*> SplineMeshes;

public:	
	// Sets default values for this actor's properties
	AThrowableProjectileIndicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "ProjectileIndicator")
	void UpdateIndicatorLine(TArray<FVector> _vectors);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ProjectileIndicator", meta = (DisplayName = "UpdateIndicatorLine"))
	void K2_UpdateIndicaterLine(const TArray<FVector>& _vectors);

	/** Getter */
	FORCEINLINE class USplineComponent* GetSplineComponent() const { return SplineComponent; }


};
