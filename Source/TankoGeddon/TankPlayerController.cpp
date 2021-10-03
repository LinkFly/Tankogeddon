// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"

void ATankPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("MoveForward"), this, &ATankPlayerController::MoveForward);
}

void ATankPlayerController::MoveForward(float InAxisValue)
{
	//FVector location = GetActorLocation();
	auto tank = Cast<ATankPawn>(GetPawn());
	if (tank) {
		tank->MoveForward(InAxisValue);
	}
}
