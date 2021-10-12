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
	
	CurrentMoveForwardAxis = FMath::FInterpTo(CurrentMoveForwardAxis, TargetMoveForwardAxis, DeltaTime, MovementSmoothness);
	FVector direction = GetActorForwardVector() * CurrentMoveForwardAxis;
	FVector newActorLocation = GetActorLocation() + direction * MoveSpeed;
	SetActorLocation(newActorLocation, true);
	CurrentRotateRightAxis = FMath::FInterpTo(CurrentRotateRightAxis, TargetRotateRightAxis, DeltaTime, RotationSmoothness);
	float rotation = GetActorRotation().Yaw + CurrentRotateRightAxis * RotationSpeed;
	SetActorRotation(FRotator{0, rotation, 0});

	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TurretTargetPosition);
	float TargetRotationYaw = targetRotation.Yaw;
	FRotator curRotation = TurretMesh->GetComponentRotation();

	//float curRotationYaw = curRotation.Yaw;
	//float targetRotationYaw = targetRotation.Yaw;
	//if (FMath::Sign(curRotationYaw) != FMath::Sign(targetRotationYaw)
	//	&& FMath::Abs(curRotationYaw - targetRotationYaw) > 180.f) {
	//	targetRotationYaw += FMath::Sign(curRotationYaw) * 360.f;
	//}
	//curRotation.Yaw = FMath::FInterpTo(curRotationYaw, targetRotationYaw, DeltaTime, TurretRotationSmoothness);
	//TurretMesh->SetWorldRotation(curRotation);
	targetRotation.Pitch = curRotation.Pitch;
	targetRotation.Roll = curRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::RInterpTo(curRotation, targetRotation, DeltaTime, TurretRotationSmoothness));
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


void ATankPawn::FireSpecial()
{
	if (Cannon) Cannon->FireSpecial();
}

void ATankPawn::SetupCannon(TSubclassOf<class ACannon> NewCannonClass)
{	
	TSubclassOf<class ACannon> curCannonClass;
	if (NewCannonClass == nullptr) {
		curCannonClass = CannonClass;
	}
	else {
		curCannonClass = NewCannonClass;
	}

	if (Cannon) {
		Cannon->Destroy();
	}
	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(curCannonClass, params);
	Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}