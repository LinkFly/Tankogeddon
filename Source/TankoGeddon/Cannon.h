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
	float FireRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float FireRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float FireDamage = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	ECannonType Type = ECannonType::FireProjectile;

	FTimerHandle ReloadTimerHandle;

	bool bIsReadyToFire = false;

public:	
	// Sets default values for this actor's properties
	ACannon();

	void Fire();

	bool IsReadyToFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	void Reload();
};
