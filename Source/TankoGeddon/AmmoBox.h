// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThingBox.h"
#include "AmmoBox.generated.h"

class ATankPawn;

UCLASS()
class TANKOGEDDON_API AAmmoBox : public AThingBox
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoBox();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	TSubclassOf<class ACannon> CannonClass;

	virtual void ActionWithTank(ATankPawn* Tank) override;
};
