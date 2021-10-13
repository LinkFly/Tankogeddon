// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThingBox.h"
#include "BullitsBox.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ABullitsBox : public AThingBox
{
	GENERATED_BODY()

public:
	ABullitsBox();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	int32 BullitsCount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	class UTextRenderComponent* Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	class UArrowComponent* TextPoint;
	
	virtual void BeginPlay() override;
private:
	void ActionWithTank(class ATankPawn* Tank) override;
};
