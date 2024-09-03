#pragma once
#include "AbilitySystemComponent.h"
#include "KrzyweKarty2/AbilitySystem/Attributes/KKAttributeSet.h"

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

UCLASS(BlueprintType) // originally a struct, but BP events don't support passing structs/primitives by reference
class UAttackInfo : public UObject // any info about an attack that can be useful
{
	GENERATED_BODY()
public:

	UAttackInfo() { };
	
	UPROPERTY(BlueprintReadWrite)
	EAttackType AttackType = EAttackType::DefaultAttack;

	UPROPERTY(BlueprintReadWrite)
	float Damage = 0; // set by character

	UPROPERTY(BlueprintReadWrite)
	uint8 AttackLevel = 0; // applies when AttackType is Ability
};

USTRUCT(BlueprintType)
struct FAbilityCost
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayAttribute AbilityCostAttribute = UKKAttributeSet::GetManaAttribute();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AbilityCost = 0;

	bool CheckIfCanAfford(const UAbilitySystemComponent* AbilitySystemComponent) const
	{
		return AbilitySystemComponent->GetNumericAttribute(AbilityCostAttribute) >= AbilityCost;
	}

	void CommitAbility(UAbilitySystemComponent* AbilitySystemComponent) const
	{
		AbilitySystemComponent->ApplyModToAttribute(AbilityCostAttribute, EGameplayModOp::Additive, -AbilityCost);
	}
};

USTRUCT(BlueprintType)
struct FAbilityDescription
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> AbilityImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AbilityDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ShowOnlyInnerProperties))
	FAbilityCost AbilityCost;
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

	static TMap<FRelativeDirection, TArray<FRelativeDirection>> SortDirections(const TArray<FRelativeDirection>& InRelativeDirections)
	{
		TMap<FRelativeDirection, TArray<FRelativeDirection>> SortedDirections;

		for (const FRelativeDirection& Direction : InRelativeDirections)
		{
			FRelativeDirection NormalizedDirection = Direction.Normalize();
			
			SortedDirections.FindOrAdd(NormalizedDirection).Add((Direction));
		}

		return SortedDirections;
	}

	bool operator==(const FRelativeDirection& Other) const
	{
		return VerticalDirection == Other.VerticalDirection && HorizontalDirection == Other.HorizontalDirection;
	}

	bool operator<(const FRelativeDirection& Other) const
	{
		return FMath::Abs(VerticalDirection) < FMath::Abs(Other.VerticalDirection) || FMath::Abs(HorizontalDirection) < FMath::Abs(Other.HorizontalDirection);
	}

	friend uint32 GetTypeHash(const FRelativeDirection& RelativeDirection)
	{
		return HashCombine(GetTypeHash(RelativeDirection.VerticalDirection), GetTypeHash(RelativeDirection.HorizontalDirection));
	}
};
