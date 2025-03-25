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
	
	// ---------------------------------------------------
	// ATTRIBUTES CAPTURE
	float TargetHealth;
	float TargetDefence;
	
	CaptureAttribues(ExecutionParams, TargetHealth, TargetDefence);
	
	const float HealthDamage = FMath::Max(TargetCharacter->GetStrength() - TargetDefence, 0.f);
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
