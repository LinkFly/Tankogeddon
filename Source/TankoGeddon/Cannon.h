// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include "GameStructs.h"
#include <Engine/EngineTypes.h>
#include "Cannon.generated.h"

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	int32 AmmoCount = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float FireRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float FireRateSpecial = .5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "Type == ECannonType::FireTrace", EditConditionHides),
		Category = "FireParams")
	float FireRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float FireDamage = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = 1), Category = "FireParams")
	int32 ShotsInSeries = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, 
		meta = (EditCondition = "ShotsInSeries > 1", EditConditionHides), 
		Category = "FireParams")
	float TimeBetweenSeriesOfShots = .2f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	ECannonType Type = ECannonType::FireProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "Type == ECannonType::FireProjectile", EditConditionHides), 
		Category = "FireParams")
	TSubclassOf<class AProjectile> ProjectileClass;

	FTimerHandle ReloadTimerHandle;
	TArray< FTimerHandle* > TimerHandlesForSeriesOfShots;

	bool bIsReadyToFire = false;

	

public:	
	// Sets default values for this actor's properties
	ACannon();

	void Shot(bool bSpecial);
	void SimpleShot() { Shot(false); }
	void SpecialShot() { Shot(true); }
	void Fire();
	void FireSpecial();
	bool IsReadyToFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	void Reload();
private:
	void ClearTimersForSeries();
};
