#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GameStructs.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8 {
	FireProjectile = 0 UMETA(DisplayName = "Use Projectile"),
	FireTrace = 1 UMETA(DisplayName = "Use Trace"),
};

USTRUCT(BlueprintType)
struct FDamageData {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Live")
	int32 DamageValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causing")
	class AActor* Instigator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Causing")
	class AActor* DamageMaker;


};