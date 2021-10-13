// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorsPull.h"
#include "TankoGeddon.h"
#include <Kismet/GameplayStatics.h>
#include "Modules/ModuleManager.h"
#include "Core/Public/Logging/LogMacros.h"

DEFINE_LOG_CATEGORY(LogActorsPull)
//DEFINE_LOG_CATEGORY(LogActorsPull2)
//UActorsPull::DEFINE_LOG_CATEGORY(LogActorsPull)

UActorsPull::UActorsPull()
{
	
}

AActor* UActorsPull::ReceiveActor(TSubclassOf<AActor> ActorClass, 
	const FVector& Location, const FRotator& Rotation)
{
	
	//UE_LOG(LogActorsPull2, Log, TEXT("---------- TEST LogActorsPull2 ---------"));
	UE_LOG(LogActorsPull, Log, TEXT("Receive actor by class: %s"), *ActorClass->GetName());
	AActor* pActor = TryGetActorFromPool(ActorClass);
	//pActor->SetActorTransform()
	if (pActor) {
		UE_LOG(LogActorsPull, Log, TEXT("GetFrom pull: %s"), *pActor->GetName());
	}
	else UE_LOG(LogActorsPull, Log, TEXT("not found into pull"));
	//bool bFromPull = false;
	if (!pActor) {
		SubPullFill(ActorClass);
		pActor = TryGetActorFromPool(ActorClass);
		if (!pActor) {
			UE_LOG(LogActorsPull, Error, TEXT("Failed get actor from pull after reinit"));
			return nullptr;
		}
		if (pActor->IsPendingKill()) {
			UE_LOG(LogActorsPull, Error, TEXT("Failed get actor from pull reinit after reinit (actor started is killed)"));
			return nullptr;
		}
	}
	SetLocationAndRotation(pActor, Location, Rotation);
	ActivateActor(pActor);
	return pActor;
}

bool UActorsPull::ReturnActor(AActor* Actor)
{
	UE_LOG(LogActorsPull, Log, TEXT("Return to pull: %s"), *Actor->GetName());
	bool bAddedRes = TryAddPull(Actor->GetClass(), Actor);
	if (!bAddedRes) {
		Actor->Destroy();
		return false;
	}
	DisactivateActor(Actor);
	return true;
}

FString UActorsPull::GetPullDescription()
{
	static FString newLine{"\n"};
	static FString ending{newLine + ":"};
	static FString tab{"\t"};
	FString res = FString("------ ActorsPull state: ------\n");
	int curIdxSubPull = 0;
	for (auto& pair : Pull) {
		++curIdxSubPull;
		res += pair.Key->GetName() + ending;
		for (AActor* actor : pair.Value) {
			res += tab + actor->GetName();
			if (curIdxSubPull != Pull.Num() - 1)
				res += newLine;
		}
	}
	return res += TEXT("----------------------------\n");
}

void UActorsPull::SubPullFill(TSubclassOf<AActor> ActorClass)
{
	auto pSubPull = Pull.Find(ActorClass);
	if (!pSubPull) {
		pSubPull = &Pull.Add(ActorClass, TArray<AActor*>{});
	}
	UE_LOG(LogActorsPull, Log, TEXT("Fill pull, added: %d"), MaxSubPullSize - pSubPull->Num());
	int32 needToAddCount = MaxSubPullSize - pSubPull->Num();
	for (int32 i = 0; i < needToAddCount; ++i) {
		UE_LOG(LogActorsPull, Log, TEXT("Added actor N: %d"), i);
		auto newActor = AddNewActorForPull(ActorClass);
		pSubPull->Add(newActor);
	}
}

void UActorsPull::Init(int32 SubPullsCount, int32 SubPullSize, bool bEmptyPullIfNotEmpty)
{
	if (Pull.Num() != 0) {
		if (bEmptyPullIfNotEmpty) {
			Clear();
		}
		else if (Pull.Num() != 0)
		{
			UE_LOG(LogActorsPull, Error, TEXT("Failed init pull when it busy (parameter bEmptyPullIfNotEmpty = false)"));
			return;
		}
	}
	MaxSubPullsCount = SubPullsCount;
	MaxSubPullSize = SubPullSize;
}

AActor* UActorsPull::AddNewActorForPull(TSubclassOf<AActor> ActorClass)
{
	UWorld* world = GetWorld();
	AActor* res = nullptr;
	if (world) {
		FActorSpawnParameters spawnParams;
		res = world->SpawnActor<AActor>(ActorClass);
		if (!res) {
			UE_LOG(LogActorsPull, Error, TEXT("Failed spawn actor by ActorsPull"));
			return nullptr;
		}
		DisactivateActor(res);
	}
	return res;
}

AActor* UActorsPull::TryGetActorFromPool(TSubclassOf<AActor> ActorClass)
{
	auto pSubPull = Pull.Find(ActorClass);
	if (!pSubPull) {
		pSubPull = &Pull.Add(ActorClass, TArray<AActor*>{});
	}
	if (pSubPull->Num() > 0) {
		return pSubPull->Pop(false);
	} else return nullptr;
}

bool UActorsPull::TryAddPull(TSubclassOf<AActor> ActorClass, AActor* Actor)
{
	auto pSubPull = Pull.Find(ActorClass);
	if (!pSubPull) {
		if (Pull.Num() < MaxSubPullsCount) {
			pSubPull = &Pull.Add(ActorClass, TArray<AActor*>{});
		}
		else {
			UE_LOG(LogActorsPull, Log, TEXT("Pull filled"));
			return false;
		}
	}
	if (pSubPull->Num() >= MaxSubPullSize) {
		UE_LOG(LogActorsPull, Log, TEXT("SubPull filled"));
		return false;
	}
	pSubPull->Add(Actor);
	return true;
}

void UActorsPull::ActivateActor(AActor* Actor)
{
	Actor->SetActorEnableCollision(true);
	Actor->SetActorTickEnabled(true);
	Actor->SetActorHiddenInGame(false);
	//Actor->SetActorHiddenInGame(false);
	//Actor->SetActorHiddenInGame(false);
}

void UActorsPull::DisactivateActor(AActor* Actor)
{
	Actor->SetActorHiddenInGame(true);
	Actor->SetActorTickEnabled(false);
	Actor->SetActorEnableCollision(false);
}

void UActorsPull::SetLocationAndRotation(AActor* Actor, const FVector& Location, const FRotator& Rotation)
{
	Actor->SetActorLocation(Location);
	Actor->SetActorRotation(Rotation);
}

void UActorsPull::Clear()
{
	for (auto& pair : Pull) {
		auto& actors = pair.Value;
		for (AActor* actor : actors) {
			actor->Destroy();
		}
	}
	Pull.Empty();
}

