// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <Engine/EngineTypes.h>
#include "TankoGeddon.h"
#include "Projectile.h"
#include <DrawDebugHelpers.h>

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Scene;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void ACannon::Shot(bool bSpecial)
{
	FString extra;
	if (bSpecial)
		extra = TEXT(" [SPECIAL]");
	extra += TEXT(" Ammo left: ") + FString::FromInt(AmmoCount);

	if (Type == ECannonType::FireProjectile) {
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3, FColor::Green, *(TEXT("Cannon: FireProjectile") + extra));
		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (projectile)
			projectile->Start();
	}
	else if (Type == ECannonType::FireTrace) {
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3, FColor::Green, *(TEXT("Cannon: FireTrace") + extra));
		FHitResult outHit;
		FVector traceStart = ProjectileSpawnPoint->GetComponentLocation();
		FVector traceEnd = traceStart + ProjectileSpawnPoint->GetForwardVector() * FireRange;
		FCollisionQueryParams traceParams(FName(TEXT("FireTrace")), true, this);
		traceParams.bReturnPhysicalMaterial = false;
		bool res = GetWorld()->LineTraceSingleByChannel(outHit, traceStart, traceEnd,
			(ECollisionChannel::ECC_GameTraceChannel1), traceParams);
		if (res) {
			traceEnd = outHit.Location;
			if (outHit.Actor.IsValid() && outHit.Component.IsValid()
				&& outHit.Component->GetCollisionObjectType() == ECollisionChannel::ECC_Destructible) {
				outHit.Actor->Destroy();
			}
		}
		DrawDebugLine(GetWorld(), traceStart, traceEnd, FColor::Green, false, .5f, 0, 5.f);
	}
	
}

void ACannon::Fire()
{
	if (!IsReadyToFire()) return;

	bIsReadyToFire = false;
	SimpleShot();
	UWorld* world = GetWorld();
	if (world) {
		world->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
	}
}

void ACannon::FireSpecial()
{
	if (!IsReadyToFire()) return;
	bIsReadyToFire = false;
	if (AmmoCount == 0) return;
	--AmmoCount;
	
	UWorld* world = GetWorld();
	if (world) {
		SpecialShot();
		for (int32 i = 1; i < FMath::Min(AmmoCount, ShotsInSeries); ++i) {
			FTimerHandle* pTimerHandle = new FTimerHandle{};
			TimerHandlesForSeriesOfShots.Add(pTimerHandle);
			world->GetTimerManager().SetTimer(*pTimerHandle, this, &ACannon::SpecialShot, TimeBetweenSeriesOfShots * i, false);
		}
		float timeToFireUnlock = FMath::Max(TimeBetweenSeriesOfShots * ShotsInSeries, 1.f / FireRateSpecial);
		world->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, timeToFireUnlock, false);
	}
}

bool ACannon::IsReadyToFire()
{
	return bIsReadyToFire;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
	bIsReadyToFire = true;
}

void ACannon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	ClearTimersForSeries();
}

void ACannon::Reload()
{
	ClearTimersForSeries();
	bIsReadyToFire = true;

}

void ACannon::ClearTimersForSeries()
{
	if (TimerHandlesForSeriesOfShots.Num() > 0) {
		for (auto timerHandle : TimerHandlesForSeriesOfShots) {
			GetWorld()->GetTimerManager().ClearTimer(*timerHandle);
			delete timerHandle;
		}
		TimerHandlesForSeriesOfShots.Empty();
	}
}
