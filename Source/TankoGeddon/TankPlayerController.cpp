// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"
#include "TankoGeddon.h"
#include <DrawDebugHelpers.h>
#include <Components/StaticMeshComponent.h>

void ATankPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	
	bShowMouseCursor = true;

	InputComponent->BindAxis(TEXT("MoveForward"), this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis(TEXT("RotateRight"), this, &ATankPlayerController::RotateRight);
	InputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATankPlayerController::Fire);
	InputComponent->BindAction(TEXT("FireSpecial"), IE_Pressed, this, &ATankPlayerController::FireSpecial);
}

void ATankPlayerController::BeginPlay()
{
	auto tank = Cast<ATankPawn>(GetPawn());
	if (tank) {
		Tank = tank;
	} else UE_LOG(LogTemp, Error, TEXT("Bad cast to ATankPawn"));
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(!Tank) return;
	FVector worldMousePos, worldMouseDir;
	DeprojectMousePositionToWorld(worldMousePos, worldMouseDir);
	auto tankLocation = Tank->GetActorLocation();
	worldMousePos.Z = tankLocation.Z;
	FVector TurretTargetDirection = worldMousePos - tankLocation;
	TurretTargetDirection.Normalize();
	FVector TurretTargetPosition = tankLocation + TurretTargetDirection * 500;
	DrawDebugLine(GetWorld(), tankLocation, TurretTargetPosition, FColor::Red, false, .1f, .0f, 3.f);
	Tank->SetTurretTargetPosition(TurretTargetPosition);
}	

void ATankPlayerController::MoveForward(float InAxisValue)
{
	if (Tank) {
		Tank->MoveForward(InAxisValue);
	}
}

void ATankPlayerController::RotateRight(float InAxisValue)
{
	if (Tank) {
		Tank->RotateRight(InAxisValue);
	}
}

void ATankPlayerController::Fire()
{
	if (Tank) Tank->Fire();
}

void ATankPlayerController::FireSpecial()
{
	if (Tank) Tank->FireSpecial();
}

