// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankogeddonGameModeBase.h"
#include "Projectile.generated.h"

UCLASS()
class TANKOGEDDON_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	static AProjectile* CreateInstance(AActor* Owner, TSubclassOf<AProjectile> ProjectileClass, 
		const FVector& Location, const FRotator& Rotation);

	static bool ReleaseInstance(AActor* Owner, AProjectile* Projectile);

	// Sets default values for this actor's properties
	AProjectile();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float FireRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Influence")
	float Damage = 1.f;

	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	UFUNCTION()
	void OnMeshHit(class UPrimitiveComponent* HitComponent, 
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void Move();

	static ATankoGeddonGameModeBase* GetCurrentGameMode(AActor* Owner);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	FVector StartPosition;
};
