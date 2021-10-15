// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStructs.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMakeDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedHealth, int32, DamageValue);

UCLASS( BlueprintType, Blueprintable)
class TANKOGEDDON_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health values");
	int32 MaxHealth = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FOnMakeDeath OnMakeDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FOnChangedHealth OnChangedHealth;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(const FDamageData& DamageData);

	UFUNCTION(BlueprintCallable, Category = "Health")
	int32 GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthState() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(int32 AdditionalHealthValue);

	virtual void BeginPlay() override;
protected:
	UPROPERTY(VisibleAnywhere)
	int32 CurrentHealth = 0.f;
};
