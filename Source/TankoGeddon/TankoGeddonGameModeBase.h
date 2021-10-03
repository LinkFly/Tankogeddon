// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankoGeddonGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankoGeddonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	/*virtual void BeginPlay() override {
		Super::BeginPlay();
	}*/
	
	UFUNCTION(BlueprintCallable, Category = "Test")
	void Test() {}
};
