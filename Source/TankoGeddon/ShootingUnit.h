// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Damageable.h"
#include "ShootingUnit.generated.h"

UCLASS()
class TANKOGEDDON_API AShootingUnit : public APawn, public IDamageable
{
	GENERATED_BODY()

public:
//	// Sets default values for this pawn's properties
	AShootingUnit();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	class UHealthComponent* Health;

	void RotatingToTarget(USceneComponent* Component, const FVector& TargetPosition,
		float Speed, bool bUseConstInterpFunc = false);

	virtual void TakeDamageData_Implementation(const FDamageData& DamageData) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnChangedHealth(int32 DamageValue);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnMakeDeath();

};

