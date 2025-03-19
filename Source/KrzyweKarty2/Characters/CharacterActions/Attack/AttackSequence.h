// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackComponent.h"
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
};

UCLASS(BlueprintType)
class KRZYWEKARTY2_API UAttackSequence : public UObject
{
	GENERATED_BODY()

public:
	
	UAttackSequence();

	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn="true"))
	FAttackContext AttackContext;

	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn="true"))
	TSubclassOf<UGameplayEffect> AttackGameplayEffectClass;
	
	UPROPERTY(BlueprintReadWrite)
	float Damage;
	
protected:
	UFUNCTION(BlueprintCallable)
	void BeginAttack();
	
	void ExecuteAttackStage(EAttackStage Stage);

	TMap<EAttackStage, TArray<UAttackComponent*>> AttackPipeline;

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE AKKCharacter* GetAttacker() const { return AttackContext.Attacker; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE AKKCharacter* GetVictim() const { return AttackContext.Victim; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE FGameplayTag GetAttackType() const { return AttackContext.AttackType; }
};
