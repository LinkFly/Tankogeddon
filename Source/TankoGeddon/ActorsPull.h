// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActorsPull.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogActorsPull, Log, All)

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UActorsPull : public UObject
{
	GENERATED_BODY()
	//DECLARE_LOG_CATEGORY_CLASS(LogActorsPull, Log, All)
	//DEFINE_LOG_CATEGORY(LogActorsPull)
public:
	UActorsPull();
	int32 MaxSubPullsCount = 10;
	// Pull for one kind of class (inheritance AActor)
	int32 MaxSubPullSize = 100;
	TMap<TSubclassOf<AActor>, TArray<AActor*>> Pull;
	AActor* ReceiveActor(TSubclassOf<AActor> ActorClass,
		const FVector& Location, const FRotator& Rotation);
	bool ReturnActor(AActor* Actor);
	FString GetPullDescription();
	void SubPullFill(TSubclassOf<AActor> ActorClass);
	void Init(int32 SubPullsCount, int32 SubPullSize, bool bEmptyPullIfNotEmpty = true);
protected:
	AActor* AddNewActorForPull(TSubclassOf<AActor> ActorClass);
	AActor* TryGetActorFromPool(TSubclassOf<AActor> ActorClass);
	bool TryAddPull(TSubclassOf<AActor> ActorClass, AActor* Actor);
	void ActivateActor(AActor* Actor);
	void DisactivateActor(AActor* Actor);
	void SetLocationAndRotation(AActor* Actor, const FVector& Location, const FRotator& Rotation);
private:
	void Clear();
};
