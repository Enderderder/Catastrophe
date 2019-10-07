#pragma once

#include "CoreMinimal.h"
#include "ItemSack.h"
#include "TomatoSack.generated.h"

UCLASS()
class CATASTROPHE_API ATomatoSack : public AItemSack
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ATomatoSack();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ItemSack")
	TSubclassOf<class ATomato> TomatoClass;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	/**
	 * Called to throw a tomato from the sack
	 * @author James Johnstone
	 */
	virtual void UseItem() override;
};
