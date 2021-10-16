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
#include "HealthComponent.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = .005f;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	//TurretMesh->SetupAttachment(BodyMesh);
	TurretMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollider"));
	HitCollider->SetupAttachment(TurretMesh);

	if (auto tempMesh = LoadObjectFromGamePath<UStaticMesh>(BodyMeshPath))
		BodyMesh->SetStaticMesh(tempMesh);

	if (auto tempMesh = LoadObjectFromGamePath<UStaticMesh>(TurretMeshPath))
		TurretMesh->SetStaticMesh(tempMesh);

	BodyMesh->OnComponentHit.AddDynamic(this, &ATurret::OnHitBody);
	TurretMesh->OnComponentHit.AddDynamic(this, &ATurret::OnHitTurret);

}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters params;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, params);
	//Cannon->SetActorScale3D(GetActorScale3D());
	
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//Cannon->SetActorScale3D(FVector{ 0.187500, 0.187500, 0.162500 });

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void ATurret::Destroyed()
{
	Super::Destroyed();

	DestroyThisUnit({Cannon});

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
	RotatingToTarget(TurretMesh, PlayerPawn->GetActorLocation(), TargetingSpeed, true);
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
	if (Cannon && bEnableFire) {
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

void ATurret::OnHitTurret(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Red, TEXT("-- Turret HIT Body---"));
}

void ATurret::OnHitBody(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Red, TEXT("-- Turret HIT Turret---"));
}


