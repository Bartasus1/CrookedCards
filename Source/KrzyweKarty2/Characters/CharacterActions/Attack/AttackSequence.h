// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackComponent.h"
#include "ControlFlow.h"
#include "ControlFlowNode.h"
#include "GameplayTagContainer.h"

#include "UObject/Object.h"
#include "AttackSequence.generated.h"


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
	FGameplayTag AttackType;

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

	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn="true"))
	FAttackContext AttackContext;

	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn="true"))
	TSubclassOf<UGameplayEffect> AttackGameplayEffectClass;
	
	UPROPERTY(BlueprintReadWrite)
	float Damage;
	
protected:
	
	void ExecuteAttackStage(FControlFlowNodeRef FlowHandle, EAttackStage Stage);
	void ExecuteDamage(FControlFlowNodeRef FlowHandle);

	void GatherAttackComponents(const AKKCharacter* Character);

private:
	TMap<EAttackStage, TArray<UAttackComponent*>> AttackPipeline;

	TSharedPtr<FControlFlow> AttackFlow;

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE AKKCharacter* GetAttacker() const { return AttackContext.Attacker; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE AKKCharacter* GetVictim() const { return AttackContext.Victim; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE FGameplayTag GetAttackType() const { return AttackContext.AttackType; }
};
