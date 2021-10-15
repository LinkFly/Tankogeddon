// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include "TankoGeddon.h"
#include "Damageable.h"
#include "GameStructs.h"
#include <Kismet/GameplayStatics.h>
#include <Components/PrimitiveComponent.h>
#include "TankogeddonGameModeBase.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = .005f;

	//USceneComponent* scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//RootComponent = scene;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//Mesh->SetupAttachment(scene);
	RootComponent = Mesh;

	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	Mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnMeshHit);
	UE_LOG(LogTankoGeddon, Log, TEXT("Created (constructor): %s"), *GetName());
}

void AProjectile::OnMeshHit(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Green, 
		//"Hit. Actor Name = " + OtherActor->GetName() + ", OtherComp Name = " + OtherComp->GetName());
	if (OtherActor == GetInstigator()) {
		ReleaseInstance(this, this);
		return;
	}

	ATankoGeddonGameModeBase::GetCurrentGameMode(this)->CheckingAndDamage(Damage, this, GetInstigator(), OtherActor, OtherComp);

	ReleaseInstance(this, this);
}

void AProjectile::Move()
{

}

ATankoGeddonGameModeBase* AProjectile::GetCurrentGameMode(AActor* Owner)
{
	return ATankoGeddonGameModeBase::GetCurrentGameMode(Owner);
}

/*
// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}
*/
// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * DeltaTime;
	SetActorLocation(nextPosition, true);

	if (FVector::Dist(GetActorLocation(), StartPosition) > FireRange) {
		UE_LOG(LogTankoGeddon, Log, TEXT("Destroy on range: %s"), *GetName());
		//Destroy();
		ReleaseInstance(this, this);
	}
}


AProjectile* AProjectile::CreateInstance(AActor* Owner, TSubclassOf<AProjectile> ProjectileClass, const FVector& Location, const FRotator& Rotation)
{
	//return GEngine->GameViewport->GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation);
	auto gameMode = GetCurrentGameMode(Owner);
	if (!gameMode) return nullptr;
	return Cast<AProjectile>(gameMode->ReceiveActor(ProjectileClass, Location, Rotation));
}

bool AProjectile::ReleaseInstance(AActor* Owner, AProjectile* Projectile)
{
	//return Projectile->Destroy();
	auto gameMode = GetCurrentGameMode(Owner);
	if (!gameMode) return nullptr;
	return gameMode->ReturnActor(Projectile);
}
