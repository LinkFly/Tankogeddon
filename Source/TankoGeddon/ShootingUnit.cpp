// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingUnit.h"
#include "HealthComponent.h"
#include <Kismet/KismetMathLibrary.h>

//// Sets default values
AShootingUnit::AShootingUnit()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	Health->OnChangedHealth.AddDynamic(this, &AShootingUnit::OnChangedHealth);
	Health->OnMakeDeath.AddDynamic(this, &AShootingUnit::OnMakeDeath);

}

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
	Health->TakeDamage(DamageData);
}

void AShootingUnit::DestroyThisUnit(const TArray<AActor*>& CreatedActors)
{
	for (auto& child : CreatedActors) {
		if (child && !child->IsPendingKill()) {
			child->Destroy();
		}
	}
}

void AShootingUnit::OnChangedHealth_Implementation(int32 DamageValue)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Red,
		TEXT("[PARENT]Damage: ") + FString::FromInt(DamageValue)
		+ TEXT(" Health left: ") + FString::FromInt(Health->GetHealth()));
}

void AShootingUnit::OnMakeDeath_Implementation()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Red, TEXT("ParentOnDie"));
	Destroy();
}
