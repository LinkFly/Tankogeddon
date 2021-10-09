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
	/*float x, y;
	auto bPosRes = GetMousePosition(x, y);
	UE_LOG(TankoGeddon, Log, TEXT("x: %f, y: %f"), x, y);*/
	FVector worldMousePos, worldMouseDir;
	DeprojectMousePositionToWorld(worldMousePos, worldMouseDir);
	//UE_LOG(TankoGeddon, Log, TEXT("worldMousePos: %s, worldMouseDir: %s"), *worldMousePos.ToString(), *worldMouseDir.ToString());

	auto tankLocation = Tank->GetActorLocation();
	FVector TurretTargetDirection = worldMousePos - tankLocation;
	TurretTargetDirection.Z = 0.f;
	TurretTargetDirection.Normalize();
	FVector TurretTargetPosition = tankLocation + TurretTargetDirection * 500;
	//UE_LOG(TankoGeddon, Log, TEXT("tank: %s, dir: %s"), *tankLocation.ToString(), *TurretTargetDirection.ToString());
	DrawDebugLine(GetWorld(), tankLocation, TurretTargetPosition, FColor::Red, false, .1f, 5);
	
	
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

