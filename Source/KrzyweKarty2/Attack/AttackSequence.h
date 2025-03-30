// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackComponent.h"
#include "GameplayEffectTypes.h"

#include "UObject/Object.h"
#include "AttackSequence.generated.h"


class FControlFlow;
class AKKCharacter;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FAttackContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	AKKCharacter* Attacker;

	UPROPERTY(BlueprintReadWrite)
	AKKCharacter* Victim;

	UPROPERTY(BlueprintReadWrite)
	EAttackType AttackType = EAttackType::DefaultAttack;

	UPROPERTY(BlueprintReadWrite)
	int32 AbilityIndex = -1;

	bool IsValid() const
	{
		return Attacker && Victim;
	}

	bool DoesCharacterMatchRole(const AKKCharacter* Character, EAttackRole AttackRole) const
	{
		switch (AttackRole)
		{
		case EAttackRole::Attacker:
			return Character == Attacker;
		case EAttackRole::Victim:
			return Character == Victim;
		default:
			return false;
		}
	}
};

UCLASS(BlueprintType)
class KRZYWEKARTY2_API UAttackSequence : public UObject
{
	GENERATED_BODY()

public:
	
	UAttackSequence();

	UFUNCTION(BlueprintCallable)
	void BeginAttack();

	UFUNCTION(BlueprintCallable)
	void StopExecution();

	UFUNCTION(BlueprintCallable)
	void ModifyDamage(int32 InDamage, EGameplayModOp::Type ModificationType = EGameplayModOp::Override);

private:

	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn="true", AllowPrivateAccess="true"))
	FAttackContext AttackContext;
	
	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn="true", AllowPrivateAccess="true"))
	int32 Damage = -1; // option to specify damage at the beginning of the attack (other than attacker's strength - useful in abilities)
	
protected:
	
	void ExecuteAttackStage(EAttackStage Stage);
	void ExecuteDamage();
	void CheckVictimDeath();

	void GatherAttackComponents(const AKKCharacter* Character);
	bool DoesComponentMatchPrerequisites(const UAttackComponent* AttackComponent, const AKKCharacter* Character) const;

private:
	
	TMap<EAttackStage, TArray<UAttackComponent*>> AttackPipeline;

	TSharedPtr<FControlFlow> AttackFlow;
	
public:
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE AKKCharacter* GetAttacker() const { return AttackContext.Attacker; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE AKKCharacter* GetVictim() const { return AttackContext.Victim; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE EAttackType GetAttackType() const { return AttackContext.AttackType; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsAbilityAttack() const { return AttackContext.AttackType != EAttackType::DefaultAttack; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetAbilityIndex() const { return AttackContext.AttackType != EAttackType::DefaultAttack; }
};
