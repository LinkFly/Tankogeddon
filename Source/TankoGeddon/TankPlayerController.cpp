// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"

void ATankPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("MoveForward"), this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &ATankPlayerController::MoveRight);
}

void ATankPlayerController::BeginPlay()
{
	auto tank = Cast<ATankPawn>(GetPawn());
	if (tank) {
		Tank = tank;
	} else UE_LOG(LogTemp, Error, TEXT("Bad cast to ATankPawn"));
}

void ATankPlayerController::MoveForward(float InAxisValue)
{
	if (Tank) {
		Tank->MoveForward(InAxisValue);
	}
}

void ATankPlayerController::MoveRight(float InAxisValue)
{
	if (Tank) {
		Tank->MoveRight(InAxisValue);
	}
}
