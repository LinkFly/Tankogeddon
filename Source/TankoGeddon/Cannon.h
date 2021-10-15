// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include "GameStructs.h"
#include <Engine/EngineTypes.h>
#include "Cannon.generated.h"

//class ATankPawn;
UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()

	//friend class ATankPawn;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	ECannonType Type = ECannonType::FireProjectile;

	UFUNCTION()
	static int32 GetDefaultAmmoCount() { return 0; }
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	int32 AmmoCount = 20;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	int32 MaxAmmoCount = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float FireRate = .5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float FireRateSpecial = .25f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "Type == ECannonType::FireTrace", EditConditionHides),
		Category = "FireParams")
	float FireRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "Type == ECannonType::FireTrace", EditConditionHides),
		Category = "FireParams")
	int32 FireTraceDamage = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float FireDamage = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = 1), Category = "FireParams")
	int32 ShotsInSeries = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, 
		meta = (EditCondition = "ShotsInSeries > 1", EditConditionHides), 
		Category = "FireParams")
	float TimeBetweenSeriesOfShots = .05f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "Type == ECannonType::FireProjectile", EditConditionHides),
		Category = "FireParams")
	TSubclassOf<class AProjectile> ProjectileClass;

	FTimerHandle ReloadTimerHandle;
	FTimerHandle NextShotInSeries;
	int32 curShotInSeries;
	//FTimerHandle EndSeriesTimerHandle;
	//TArray< FTimerHandle* > TimerHandlesForSeriesOfShots;

	bool bIsReadyToFire = false;

	

public:	
	// Sets default values for this actor's properties
	ACannon();

	void Shot(bool bSpecial);
	void SimpleShot();
	void SpecialShot();
	void Fire();
	void FireSpecial();
	bool IsReadyToFire();
	void SetupBullits(int32 Count);
	void AddBullits(int32 Count);
	bool EqualProjectileClass(const ACannon* Other) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	void Reload();
private:
	//void ClearTimersForSeries();
	void FireGeneral(bool bSpecial);
	void PullBullitsFill();
	//void HandlerEndSeries();


};
