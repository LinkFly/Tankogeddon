// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UHealthComponent::TakeDamage(const FDamageData& DamageData)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Orange, TEXT("UHealthComponent::TakeDamage"));
	int32 TakeDamageValue = DamageData.DamageValue;
	CurrentHealth -= TakeDamageValue;
	if (CurrentHealth <= 0) {
		if (OnMakeDeath.IsBound()) {
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Orange, TEXT("UHealthComponent::TakeDamage 1"));
			OnMakeDeath.Broadcast();
		}
	}
	else {
		if (OnChangedHealth.IsBound()) {
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Orange, TEXT("UHealthComponent::TakeDamage 2"));
			OnChangedHealth.Broadcast(TakeDamageValue);
		}
	}
}

int32 UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetHealthState() const
{
	return static_cast<float>(CurrentHealth) / MaxHealth;
}

void UHealthComponent::AddHealth(int32 AdditionalHealthValue)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + AdditionalHealthValue, 0, MaxHealth);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

