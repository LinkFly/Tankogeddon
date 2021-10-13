// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PullActorsWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TANKOGEDDON_API UPullActorsWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	// Test (ignore it)
	UFUNCTION()
	void TestFuncSubsystem();
};
