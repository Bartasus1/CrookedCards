// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackHealthMagnitudeCalculation.h"
#include "KrzyweKarty2/AbilitySystem/Attributes/KKAttributeSet.h"

UAttackHealthMagnitudeCalculation::UAttackHealthMagnitudeCalculation()
{
	VictimHealthDef.AttributeToCapture = UKKAttributeSet::GetHealthAttribute();
	VictimHealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VictimHealthDef.bSnapshot = true;

	VictimDefenceDef.AttributeToCapture = UKKAttributeSet::GetDefenceAttribute();
	VictimDefenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VictimDefenceDef.bSnapshot = true;

	RelevantAttributesToCapture.Add(VictimHealthDef);
	RelevantAttributesToCapture.Add(VictimDefenceDef);
}

float UAttackHealthMagnitudeCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Health = 0.f;
	GetCapturedAttributeMagnitude(VictimHealthDef, Spec, EvaluationParameters, Health);

	float Defence = 0.f;
	GetCapturedAttributeMagnitude(VictimDefenceDef, Spec, EvaluationParameters, Defence);

	float Strength = 0.f;
	GetCapturedAttributeMagnitude(AttackerStrengthDef, Spec, EvaluationParameters, Strength);

	const float Damage = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Attack.Damage"),false, Strength);
	const float Result = Health - FMath::Max(Damage - Defence, 0.f);
	UE_LOG(LogTemp, Warning, TEXT("Dealing %f damage to the victim -> final health: %f"), Damage, Result);
	return Result;

	
}
