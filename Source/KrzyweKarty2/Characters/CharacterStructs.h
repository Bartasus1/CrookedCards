#pragma once
#include "CharacterStructs.generated.h"

class AAbilityActor;
class AKKCharacter;

USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Strength = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Defence = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Health = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Mana = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxAttackRange = 1;
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	DefaultAttack,
	ActiveAbility,
	PassiveAbility
};

USTRUCT(BlueprintType)
struct FAttackInfo // any info about an attack that can be useful
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	EAttackType AttackType;
	
};



USTRUCT(BlueprintType)
struct FAbilityDescription
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AbilityDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AbilityCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> AbilityImage;
};

USTRUCT(BlueprintType)
struct FRelativeDirection
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 VerticalDirection = 0;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 HorizontalDirection = 0;

	FRelativeDirection Normalize() const
	{
		FRelativeDirection NormalizedDirection;
		if(VerticalDirection != 0)
		{
			NormalizedDirection.VerticalDirection = VerticalDirection / FMath::Abs(VerticalDirection);
		}

		if(HorizontalDirection != 0)
		{
			NormalizedDirection.HorizontalDirection = HorizontalDirection / FMath::Abs(HorizontalDirection);
		}

		return NormalizedDirection;
	}

	bool operator==(const FRelativeDirection& Other) const
	{
		return VerticalDirection == Other.VerticalDirection && HorizontalDirection == Other.HorizontalDirection;
	}
};
