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
	//AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void ACannon::Shot(bool bSpecial)
{
	FString extra;
	if (bSpecial)
		extra = TEXT(" [SPECIAL]");

	if (Type == ECannonType::FireProjectile) {
		extra += TEXT(" Ammo left: ") + FString::FromInt(AmmoCount);
		AProjectile* projectile;
		FVector location = ProjectileSpawnPoint->GetComponentLocation();
		FRotator rotation = ProjectileSpawnPoint->GetComponentRotation();
		projectile = AProjectile::CreateInstance(this, ProjectileClass, location, rotation);
		projectile->SetInstigator(GetInstigator());
		if (!projectile) {
			UE_LOG(LogTankoGeddon, Error, TEXT("Failed get actor from pull"));
			return;
		}
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
			//if (outHit.Actor.IsValid() && outHit.Component.IsValid()
			//	&& outHit.Component->GetCollisionObjectType() == ECollisionChannel::ECC_Destructible) {
			//	outHit.Actor->Destroy();
			//}
			auto otherActor = outHit.Actor;
			auto otherComp = outHit.Component;
			if (otherActor.IsValid() && otherComp.IsValid()) {
				ATankoGeddonGameModeBase::GetCurrentGameMode(this)->CheckingAndDamage(
					FireTraceDamage, this, GetInstigator(), otherActor.Get(), otherComp.Get());
			}
		}
		DrawDebugLine(GetWorld(), traceStart, traceEnd, FColor::Green, false, .5f, 0, 5.f);
	}
	
}

void ACannon::SimpleShot()
{
	Shot(false);
	PullBullitsFill();
}

void ACannon::SpecialShot()
{
	UWorld* world = GetWorld();
	if (!world) {
		UE_LOG(LogTankoGeddon, Log, TEXT("Failed get world"));
		return;
	}
	Shot(true);
	++curShotInSeries;
	if (curShotInSeries == ShotsInSeries) {
		PullBullitsFill();
		world->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRateSpecial, false);
	}
	else {
		world->GetTimerManager().SetTimer(NextShotInSeries, this, &ACannon::SpecialShot, TimeBetweenSeriesOfShots, false);
	}
}

void ACannon::FireGeneral(bool bSpecial)
{
	if (!IsReadyToFire()) return;
	if (AmmoCount == 0) {
		//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Green, "Ammo left: " + FString::FromInt(AmmoCount));
		return;
	}
	bIsReadyToFire = false;
	--AmmoCount;

	UWorld* world = GetWorld();
	if (world) {
		if (!bSpecial) {
			SimpleShot();
			world->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
		} else {
			curShotInSeries = 0;
			SpecialShot();
		}
	}
}

void ACannon::PullBullitsFill()
{
	auto gameMode = ATankoGeddonGameModeBase::GetCurrentGameMode(this);
	if (gameMode) gameMode->PullFill(ProjectileClass);
}

void ACannon::Fire()
{
	FireGeneral(false);
}

void ACannon::FireSpecial()
{
	FireGeneral(true);
}

bool ACannon::IsReadyToFire()
{
	return bIsReadyToFire;
}

void ACannon::AddBullits(int32 Count)
{
	SetupBullits(AmmoCount + Count);
}

void ACannon::SetupBullits(int32 Count)
{
	AmmoCount = Count;
	if (MaxAmmoCount > 0) {
		AmmoCount = FMath::Clamp(AmmoCount, 1, MaxAmmoCount);
	}
	FString msg = FString::Printf(TEXT("New ammo = %d"), AmmoCount);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Green, msg);
}

bool ACannon::EqualProjectileClass(const ACannon* Other) const
{
	return Type == ECannonType::FireProjectile 
		&& Other->Type == ECannonType::FireProjectile 
		&& ProjectileClass == Other->ProjectileClass;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
	bIsReadyToFire = true;
	if (Type == ECannonType::FireProjectile) {
		ATankoGeddonGameModeBase::GetCurrentGameMode(this)->PullFill(ProjectileClass);
	}
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
