// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThingBox.generated.h"

class ATankPawn;

UCLASS(Abstract)
class TANKOGEDDON_API AThingBox : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AThingBox();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* Mesh;

	UFUNCTION()
	virtual void ActionWithTank(ATankPawn* Tank); // abstract

public:
	UFUNCTION()
		void OnMeshBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
