// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <Engine/EngineTypes.h>
#include "TankoGeddon.h"

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

void ACannon::Fire()
{
	
	if (!IsReadyToFire()) return;

	bIsReadyToFire = false;
	if (Type == ECannonType::FireProjectile) {
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3, FColor::Green, TEXT("Cannon: FireProjectile"));
	}
	else if (Type == ECannonType::FireTrace) {
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3, FColor::Green, TEXT("Cannon: FireTrace"));
	}
	
	UWorld* world = GetWorld();
	if (world) {
		world->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
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
}

void ACannon::Reload()
{
	bIsReadyToFire = true;
}
