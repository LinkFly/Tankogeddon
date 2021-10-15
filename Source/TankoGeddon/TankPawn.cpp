// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Math/UnrealMathUtility.h>
#include "TankoGeddon.h"
#include <Kismet/KismetMathLibrary.h>
#include "Cannon.h"
#include "HealthComponent.h"

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

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	Health->OnChangedHealth.AddDynamic(this, &ATankPawn::OnChangedHealth);
	Health->OnMakeDeath.AddDynamic(this, &ATankPawn::OnMakeDeath);

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

void ATankPawn::TakeDamageData_Implementation(const FDamageData& DamageData)
{
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, TEXT("-- Tank Damage --"));
	Health->TakeDamage(DamageData);
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

void ATankPawn::SwitchCannon()
{
	UE_LOG(LogTankoGeddon, Log, TEXT("Switch CannonsClasses.Num(): %d"), CannonsClasses.Num());
	if (CannonsClasses.Num() >= 2) {
		UE_LOG(LogTankoGeddon, Log, TEXT("Switch CannonsClasses.Num() > 2"));
		auto lastCannon = CannonsClasses[CannonsClasses.Num() - 1];
		for (int32 i = CannonsClasses.Num() - 1; i > 0 ; --i) {
			CannonsClasses[i] = CannonsClasses[i - 1];
		}
		CannonsClasses[0] = lastCannon;
	}
	SetupCannon();
}

void ATankPawn::SetupCannon(TSubclassOf<class ACannon> NewCannonClass)
{	
	TSubclassOf<class ACannon> curCannonClass;
	if (NewCannonClass == nullptr) {
		if (CannonsClasses.Num() == 0) {
			return;
		}
	}
	else {
		//if (CannonsClasses.Num() >= 1 && CannonsClasses.Last() == NewCannonClass) {
		//	//UE_LOG(LogTankoGeddon, Log, TEXT("if Cannon"));
		//	//if (newCannon->Type == Cannon->Type && newCannon->Type == ECannonType::FireProjectile
		//	//	&& newCannon->EqualProjectileClass(Cannon))
		//	//{
		//	//	UE_LOG(LogTankoGeddon, Log, TEXT("equal cannons"));
		//	//	SetupBullits(newCannon->AmmoCount + Cannon->AmmoCount, newCannon);
		//	//}
		//	//auto tmpObj = NewObject(nullptr, NewCannonClass);
		//	//tmpObj->Destroy();
		//	//NewCannonClass->GetDefaultAmmoCount();
		//	//return;
		//}
		if (!(CannonsClasses.Num() >= 1 && CannonsClasses.Last() == NewCannonClass))
		{
			if (CannonsClasses.Num() < MaxCannons) {
				UE_LOG(LogTankoGeddon, Log, TEXT("CannonsClasses.Num(): %d"), CannonsClasses.Num());
				CannonsClasses.Add(NewCannonClass);
			}
			else {
				CannonsClasses[CannonsClasses.Num() - 1] = NewCannonClass;
			}
		}
	}
	curCannonClass = CannonsClasses.Last();

	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	auto newCannon = GetWorld()->SpawnActor<ACannon>(curCannonClass, params);
	auto curAmmoCount = newCannon->AmmoCount;
	if (Cannon) {
		UE_LOG(LogTankoGeddon, Log, TEXT("if Cannon"));
		if (newCannon->EqualProjectileClass(Cannon))
		{
			UE_LOG(LogTankoGeddon, Log, TEXT("equal cannons"));
			curAmmoCount += Cannon->AmmoCount;
		}
		Cannon->Destroy();
	}
	newCannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Cannon = newCannon;
	SetupBullits(curAmmoCount, newCannon);
}

void ATankPawn::SetupBullits(int32 Count, ACannon* SomeCannon)
{
	ACannon* curCannon = SomeCannon ? SomeCannon : Cannon;
	if (curCannon) {
		curCannon->SetupBullits(Count);
	}
}

void ATankPawn::AddBullits(int32 Count)
{
	if (Cannon) {
		Cannon->AddBullits(Count);
	}
}

void ATankPawn::OnChangedHealth(int32 DamageValue)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, 
		TEXT("Damage = ") + FString::FromInt(DamageValue) + 
		TEXT(", Health = ") + FString::FromInt(Health->GetHealth()));
}

void ATankPawn::OnMakeDeath()
{
	Destroy();
}
