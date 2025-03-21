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

	bRequiresPassedInTags = true;
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
	
	UAttackInfo* AttackInfo = NewObject<UAttackInfo>();
	AttackInfo->AttackType = ExecutionParams.GetPassedInTags().First();
	
	if(!TargetCharacter->Target_CanBeAttacked(SourceCharacter, AttackInfo)) // target can block attacks, so no point going further
	{
		return;
	}
	
	SourceCharacter->Attacker_OnAttackBegin(TargetCharacter, AttackInfo); // notify attacker (just in case or maybe an animation or some attributes changes - that's why we do it before attributes capture)

	// ---------------------------------------------------
	// ATTRIBUTES CAPTURE
	float TargetHealth;
	float TargetDefence;
	
	CaptureAttribues(ExecutionParams, TargetHealth, TargetDefence);

	// ---------------------------------------------------
	
	SourceCharacter->Attacker_CalculateDamage(TargetCharacter, AttackInfo); // attacker calculate damage and store it in AttackInfo
	
	TargetCharacter->Target_BeforeAttackReceive(SourceCharacter, AttackInfo); // last chance for target to modify some details about an attack

	TargetCharacter->Target_OnAttackEnd(SourceCharacter, AttackInfo); // target can play some animations too after the attack
	
	SourceCharacter->Attacker_OnAttackEnd(TargetCharacter, AttackInfo); // notify attacker of a successful attack - post attack attributes modifications / buffs etc.

	
	const float HealthDamage = FMath::Max(AttackInfo->Damage - TargetDefence, 0.f);
	const float DefenceDamage = (TargetDefence > 0) ? 1.f : 0.f;
	
	if(TargetHealth - HealthDamage <= 0.f) // character has died
	{
		TargetCharacter->OnCharacterDeath.Broadcast();
		TargetCharacter->Destroy();
	}
	else
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealthProperty, EGameplayModOp::Additive, -HealthDamage));
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DefenceProperty, EGameplayModOp::Additive, -DefenceDamage));
	}
	
}

void UCharacterAttackExecCalculation::CaptureAttribues(const FGameplayEffectCustomExecutionParameters& ExecutionParams, float& Health, float& Defence) const
{
	FGameplayEffectSpec* Spec = ExecutionParams.GetOwningSpecForPreExecuteMod();

	const FGameplayTagContainer* TargetTags = Spec->CapturedTargetTags.GetAggregatedTags();
	const FGameplayTagContainer* SourceTags = Spec->CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.TargetTags = TargetTags;
	EvaluationParameters.SourceTags = SourceTags;

	Health = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealthDef, EvaluationParameters, Health);

	Defence = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DefenceDef, EvaluationParameters, Defence);
	Defence = FMath::Max(0.f, Defence);
}
