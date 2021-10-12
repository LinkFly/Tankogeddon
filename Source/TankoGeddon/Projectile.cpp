// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include "TankoGeddon.h"

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
}

void AProjectile::Start()
{
	
}

void AProjectile::OnMeshHit(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Green, "Hit");
	auto tmp = OtherComp->GetCollisionObjectType();
	if (OtherActor && OtherComp && OtherComp->GetCollisionObjectType() == ECC_Destructible) {
		OtherActor->Destroy();
	}
	Destroy();
}

void AProjectile::Move()
{

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
		Destroy();
	}
}
