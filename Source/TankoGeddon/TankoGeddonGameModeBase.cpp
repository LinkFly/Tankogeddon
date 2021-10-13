// Copyright Epic Games, Inc. All Rights Reserved.


#include "TankoGeddonGameModeBase.h"
#include "TankoGeddon.h"
#include "ActorsPull.h"
#include <Kismet/GameplayStatics.h>

ATankoGeddonGameModeBase* ATankoGeddonGameModeBase::GetCurrentGameMode(AActor* LiveActor)
{
	auto res = Cast<ATankoGeddonGameModeBase>(UGameplayStatics::GetGameMode(LiveActor->GetWorld()));
	if (!res) {
		UE_LOG(LogTankoGeddon, Error, TEXT("Failed get ATankoGeddonGameModeBase*"));
		return nullptr;
	}
	return res;
}

ATankoGeddonGameModeBase::ATankoGeddonGameModeBase()
{
	Pull = CreateDefaultSubobject<UActorsPull>(TEXT("Pull"));
	//Pull->SubPullInit(AProjectile)
}

void ATankoGeddonGameModeBase::PullFill(TSubclassOf<AActor> ActorClass)
{
	Pull->SubPullFill(ActorClass);
}

AActor* ATankoGeddonGameModeBase::ReceiveActor(TSubclassOf<AActor> ActorClass, const FVector& Location, const FRotator& Rotation)
{
	return Pull->ReceiveActor(ActorClass, Location, Rotation);
}

bool ATankoGeddonGameModeBase::ReturnActor(AActor* Actor)
{
	return Pull->ReturnActor(Actor);
}

void ATankoGeddonGameModeBase::ActorsPullDump()
{
	UE_LOG(LogTankoGeddon, Log, TEXT("%s"), *Pull->GetPullDescription());
}

void ATankoGeddonGameModeBase::PullInit(int32 SubPullsCount, int32 SubPullSize)
{
	Pull->Init(SubPullsCount, SubPullSize);
}
