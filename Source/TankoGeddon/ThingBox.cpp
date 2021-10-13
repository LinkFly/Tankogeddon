// Fill out your copyright notice in the Description page of Project Settings.


#include "ThingBox.h"
#include <Components/StaticMeshComponent.h>
#include "TankoGeddon.h"
#include "TankPawn.h"

// Sets default values
AThingBox::AThingBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName(FName("OverlapOnlyPawn"));
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AThingBox::OnMeshBeginOverlap);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetGenerateOverlapEvents(true);
}

void AThingBox::ActionWithTank(class ATankPawn* Tank)
{
	UE_LOG(LogTankoGeddon, Error, TEXT("This is abstract method. Please using child classes"));
}

void AThingBox::OnMeshBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3, FColor::Orange, TEXT("PICK"));
	ATankPawn* playerTank = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (playerTank == OtherActor) {
		ActionWithTank(playerTank);
		Destroy();
	}
}

