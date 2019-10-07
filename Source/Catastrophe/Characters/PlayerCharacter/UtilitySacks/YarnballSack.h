#pragma once

#include "CoreMinimal.h"
#include "ItemSack.h"
#include "YarnballSack.generated.h"

UCLASS()
class CATASTROPHE_API AYarnballSack : public AItemSack
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	AYarnballSack();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ItemSack")
	TSubclassOf<class AYarnBall> YarnballClass;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/**
	 * Called to throw a yarn ball from the sack
	 * @author James Johnstone
	 */
	virtual void UseItem() override;
};
