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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectileIndicator", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* EndpointMesh;

protected:

	/** Number of segmnents to form the spline, more segment means smoother spline */
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

private:

	bool bRenderingSpline = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * Update the spline points
	 * @author Richard Wulansari
	 * @param _vectors Array of vector points
	 * @note Those vectors needs to be in world space coord
	 */
	UFUNCTION(BlueprintCallable, Category = "ProjectileIndicator")
	void UpdateIndicatorLine(TArray<FVector> _vectors);

	/**
	 * Set the visibility of the spline indicator
	 * @author Richard Wulansari
	 * @param _enabled
	 */
	UFUNCTION(BlueprintCallable, Category = "ProjectileIndicator")
	void SetIndicatorEnabled(bool _enabled);

	/** Getter */
	FORCEINLINE class USplineComponent* GetSplineComponent() const { return SplineComponent; }


};
