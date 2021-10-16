// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingUnit.h"
#include "HealthComponent.h"
#include <Kismet/KismetMathLibrary.h>

//// Sets default values
//AShootingUnit::AShootingUnit()
//{
// 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//
//}
//
//// Called when the game starts or when spawned
//void AShootingUnit::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void AShootingUnit::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
//
//// Called to bind functionality to input
//void AShootingUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

void AShootingUnit::RotatingToTarget(USceneComponent* Component, const FVector& TargetPosition,
	float Speed, bool bUseConstInterpFunc)
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FRotator targetRotation =
		UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	FRotator curRotation = Component->GetComponentRotation();
	targetRotation.Pitch = curRotation.Pitch;
	targetRotation.Roll = curRotation.Roll;
	if (bUseConstInterpFunc) {
		Component->SetWorldRotation(FMath::RInterpConstantTo(curRotation, targetRotation, DeltaTime, Speed));
	}
	else {
		Component->SetWorldRotation(FMath::RInterpTo(curRotation, targetRotation, DeltaTime, Speed));
	}
}

void AShootingUnit::TakeDamageData_Implementation(const FDamageData& DamageData)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Orange,
		TEXT("[implemented] Turret is taked damage: ") + FString::FromInt(DamageData.DamageValue));
	//Damage(DamageData.DamageValue);
	Health->TakeDamage(DamageData);
}

//void AShootingUnit::TestToBaseFunction()
//{
//	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Green, TEXT("------------- [ TestToBaseFunction ] "));
//}

void AShootingUnit::OnChangedHealth_Implementation(int32 DamageValue)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Red,
		TEXT("[PARENT]Damage: ") + FString::FromInt(DamageValue)
		+ TEXT(" Health left: ") + FString::FromInt(Health->GetHealth()));
}

void AShootingUnit::OnMakeDeath_Implementation()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Red, TEXT("ParentOnDie"));
	// TODO!!! Destroy Cannon
	Destroy();
}