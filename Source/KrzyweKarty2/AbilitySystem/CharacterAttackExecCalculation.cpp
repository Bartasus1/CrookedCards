// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttackExecCalculation.h"
#include "AbilitySystemComponent.h"

#include "Attributes/KKAttributeSet.h"

#include "KrzyweKarty2/Characters/KKCharacter.h"

UCharacterAttackExecCalculation::UCharacterAttackExecCalculation()
{
	DEFINE_ATTRIBUTE_CAPTUREDEF(UKKAttributeSet, Health, Target, false);
	DEFINE_ATTRIBUTE_CAPTUREDEF(UKKAttributeSet, Defence, Target, false);
	
	RelevantAttributesToCapture.Add(HealthDef);
	RelevantAttributesToCapture.Add(DefenceDef);
}

void UCharacterAttackExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	AKKCharacter* TargetCharacter = Cast<AKKCharacter>(TargetASC->GetOwnerActor());
	AKKCharacter* SourceCharacter = Cast<AKKCharacter>(SourceASC->GetOwnerActor());

	if(!TargetASC || ! SourceASC || !TargetCharacter || !SourceCharacter) // if any ASC or character is not valid - stop
	{
		return;
	}

	FGameplayEffectSpec* Spec = ExecutionParams.GetOwningSpecForPreExecuteMod();

	const FGameplayTagContainer* TargetTags = Spec->CapturedTargetTags.GetAggregatedTags();
	const FGameplayTagContainer* SourceTags = Spec->CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.TargetTags = TargetTags;
	EvaluationParameters.SourceTags = SourceTags;
	
	float Health = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealthDef, EvaluationParameters, Health);
	
	float Defence = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DefenceDef, EvaluationParameters, Defence);
	Defence = FMath::Max(0.f, Defence);

	
	FAttackInfo AttackInfo;
	AttackInfo.AttackType = EAttackType::DefaultAttack;
	
	if(!TargetCharacter->Target_CanBeAttacked(SourceCharacter, AttackInfo))
	{
		return;
	}
	
	SourceCharacter->Attacker_OnAttackBegin(TargetCharacter, AttackInfo); // notify attacker

	float Strength = SourceCharacter->Attacker_CalculateDamage(TargetCharacter, AttackInfo); // attacker calculate damage

	TargetCharacter->Target_BeforeAttackReceive(SourceCharacter, AttackInfo, Strength); // target can modify damage based on some conditions

	
	const float Damage = FMath::Max(Strength - Defence, 0.f);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealthProperty, EGameplayModOp::Additive, -Damage));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DefenceProperty, EGameplayModOp::Additive, -1.f));
	
	if(Health - Damage <= 0.f) // character has died
	{
		TargetCharacter->OnCharacterDeath.Broadcast();
        TargetCharacter->Destroy();
	}

	SourceCharacter->Attacker_OnAttackEnd(TargetCharacter, AttackInfo);
}
