// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Math/UnrealMathUtility.h>
#include "TankoGeddon.h"
#include <Kismet/KismetMathLibrary.h>
#include "Cannon.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Canon point"));
	CannonSpawnPoint->SetupAttachment(TurretMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	SetupCannon();
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurrentMoveForwardAxis = FMath::Lerp(CurrentMoveForwardAxis, TargetMoveForwardAxis, MovementSmoothness);
	FVector direction = GetActorForwardVector() * CurrentMoveForwardAxis;

	FVector newActorLocation = GetActorLocation() + direction * MoveSpeed * DeltaTime;
	SetActorLocation(newActorLocation);

	CurrentRotateRightAxis = FMath::Lerp(CurrentRotateRightAxis, TargetRotateRightAxis, RotationSmoothness);
	float rotation = GetActorRotation().Yaw + CurrentRotateRightAxis * RotationSpeed * DeltaTime;
	SetActorRotation(FRotator{0, rotation, 0});

	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TurretTargetPosition);
	FRotator curRotation = TurretMesh->GetComponentRotation();
	targetRotation.Roll = curRotation.Roll;
	targetRotation.Pitch = curRotation.Pitch;
	TurretMesh->SetWorldRotation(FMath::Lerp(curRotation, targetRotation, TurretRotationSmoothness));
}

void ATankPawn::MoveForward(float InAxisValue)
{
	TargetMoveForwardAxis = InAxisValue;
} 

void ATankPawn::RotateRight(float InAxisValue)
{
	TargetRotateRightAxis = InAxisValue;
}

void ATankPawn::SetTurretTargetPosition(const FVector& TargetPosition)
{
	TurretTargetPosition = TargetPosition;
}

void ATankPawn::Fire()
{
	if (Cannon) Cannon->Fire();
}

void ATankPawn::SetupCannon()
{
	if (Cannon) {
		Cannon->Destroy();
	}
	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, params);
	Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}