// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

class UAttackSequence;

UENUM()
enum class EAttackStage : uint8
{
	PreAttack UMETA(DisplayName = "Pre-Attack", ToolTip="Pre eliminary attack stage. Can be used to stop it"),
	Before UMETA(DisplayName = "Before", ToolTip="Before the attack is executed - change applied damage etc."),
	After UMETA(DisplayName = "After", ToolTip="After the attack is executed - apply post attack effects"),
	MAX UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EAttackStage, EAttackStage::MAX);

UENUM()
enum class EAttackRole : uint8
{
	Victim UMETA(DisplayName = "Victim", ToolTip="The character is a victim of the attack"),
	Attacker UMETA(DisplayName = "Attacker", ToolTip="The character is an attacker")
};

UENUM()
enum class EAttackType : uint8
{
	DefaultAttack,
	Ability
};


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

	UPROPERTY(EditDefaultsOnly, meta=(InlineEditConditionToggle))
	bool AnyAttackType = false; // Turn off for any attack type

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="!AnyAttackType"))
	EAttackType AttackType = EAttackType::DefaultAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="!AnyAttackType && AttackType != EAttackType::DefaultAttack && AttackRole != EAttackRole::Victim", EditConditionHides))
	uint8 AbilityIndex = 0;

public:
	UPROPERTY(BlueprintAssignable)
	FAttackStageExecutionDelegate AttackStageExecution;

public:
	
	bool MatchesAttackType(const EAttackType InAttackType, const int32 InAbilityIndex = -1) const
	{
		const bool bMatchesAttackType = AnyAttackType || AttackType == InAttackType;
		const bool bAbilityIndexRelevant = AttackType != EAttackType::DefaultAttack && AttackRole != EAttackRole::Victim;
		const bool bMatchesAbilityIndex = !bAbilityIndexRelevant || InAbilityIndex == -1 || AbilityIndex == InAbilityIndex;

		return bMatchesAttackType && bMatchesAbilityIndex;
	}
};
