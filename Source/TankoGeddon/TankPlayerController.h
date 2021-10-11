// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	// Called to bind functionality to input
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controlled")
	class ATankPawn* Tank;

	virtual void Tick(float DeltaSeconds) override;

private:
	void MoveForward(float InAxisValue);
	void RotateRight(float InAxisValue);
	void Fire();
	void FireSpecial();
};
