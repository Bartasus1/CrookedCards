// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackProperties.h"

#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

class UAttackSequence;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttackStageExecutionDelegate, UAttackSequence*, AttackSequence);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, DisplayName="Attack Component"), DisplayName="Attack Component")
class KRZYWEKARTY2_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttackComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAttackStage AttackStage = EAttackStage::Before;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAttackRole AttackRole = EAttackRole::Attacker;

	UPROPERTY(EditDefaultsOnly, meta=(Bitmask, BitmaskEnum="EAttackType"))
	uint8 AttackType = EAttackType::DefaultAttack | EAttackType::Ability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="(AttackType & EAttackType::Ability) && AttackRole != EAttackRole::Victim", EditConditionHides))
	uint8 AbilityIndex = 0;

public:
	UPROPERTY(BlueprintAssignable)
	FAttackStageExecutionDelegate AttackStageExecution;

public:
	
	bool MatchesAttackType(const EAttackType InAttackType, const int32 InAbilityIndex = -1) const
	{
		bool bIsMatchingIndex = true;
		if(InAttackType == EAttackType::Ability && AttackRole == EAttackRole::Attacker && InAbilityIndex != -1)
		{
			bIsMatchingIndex = (AbilityIndex == InAbilityIndex);
		}
		
		return IsAttackTypePresent(InAttackType) && bIsMatchingIndex;
	}

	bool IsAttackTypePresent(const EAttackType InAttackType) const
	{
		return AttackType & InAttackType;
	}
};

