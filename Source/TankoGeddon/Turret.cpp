// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include <Components/StaticMeshComponent.h>
#include "TankoGeddon.h"
#include <Components/BoxComponent.h>
#include "Cannon.h"
#include <Components/ArrowComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <UObject/ConstructorHelpers.h>
#include <Components/SceneComponent.h>

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = .005f;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	RootComponent = BodyMesh;

	//auto Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//RootComponent = Scene;

	//BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	//BodyMesh->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	//TurretMesh->SetupAttachment(BodyMesh);
	TurretMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollider"));
	HitCollider->SetupAttachment(TurretMesh);

	//ConstructorHelpers::FObjectFinder<UStaticMesh> bodyFoundTemp(*BodyMeshPath);
	//UE_LOG(LogTankoGeddon, Log, TEXT("bodyFoundTemp.Object: %p"), bodyFoundTemp.Object);
	//if (bodyFoundTemp.Object) {
	//	BodyMesh->SetStaticMesh(bodyFoundTemp.Object);
	//}
	if (auto tempMesh = LoadObjectFromGamePath<UStaticMesh>(BodyMeshPath))
		BodyMesh->SetStaticMesh(tempMesh);

	if (auto tempMesh = LoadObjectFromGamePath<UStaticMesh>(TurretMeshPath))
		TurretMesh->SetStaticMesh(tempMesh);

	//ConstructorHelpers::FObjectFinder<UStaticMesh> turretMeshTemp(*TurretMeshPath);
	//UE_LOG(LogTankoGeddon, Log, TEXT("turretMeshTemp.Object: %p"), turretMeshTemp.Object);
	//if (turretMeshTemp.Object) {
	//	TurretMesh->SetStaticMesh(turretMeshTemp.Object);
	//}
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters params;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void ATurret::Destroyed()
{
	Super::Destroyed();

	//if (Cannon) {
	//	Cannon->Destroy();
	//}
}

void ATurret::Targeting()
{
	if (IsPlayerInRange()) {
		RotateToPlayer();
	}
	if (CanFire() && Cannon && Cannon->IsReadyToFire()) {
		Fire(); 
	}
}

void ATurret::RotateToPlayer()
{
	FRotator targetRotation = 
		UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator curRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = curRotation.Pitch;
	targetRotation.Roll = curRotation.Roll;
	TurretMesh->SetWorldRotation(
		FMath::RInterpConstantTo(curRotation, targetRotation, GetWorld()->GetDeltaSeconds(), TargetingSpeed));


}

bool ATurret::IsPlayerInRange()
{
	return FVector::DistSquared(PlayerPawn->GetActorLocation(), GetActorLocation()) < FMath::Square(TargetingRange);;
}

bool ATurret::CanFire()
{
	FVector targetingDir = TurretMesh->GetForwardVector();
	FVector dirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	dirToPlayer.Normalize();
	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDir, dirToPlayer)));
	return aimAngle <= Accuracy;
}

void ATurret::Fire()
{
	if (Cannon) {
		Cannon->Fire();
	}
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawn) {
		Targeting();
	}
	
}

