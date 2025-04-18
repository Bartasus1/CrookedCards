// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageCalculation.h"
#include "KrzyweKarty2/AbilitySystem/Attributes/KKAttributeSet.h"

UDamageCalculation::UDamageCalculation()
{
	AttackerStrengthDef.AttributeToCapture = UKKAttributeSet::GetStrengthAttribute();
	AttackerStrengthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttackerStrengthDef.bSnapshot = true;
	
	VictimDefenceDef.AttributeToCapture = UKKAttributeSet::GetDefenceAttribute();
	VictimDefenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VictimDefenceDef.bSnapshot = true;

	RelevantAttributesToCapture.Add(AttackerStrengthDef);
	RelevantAttributesToCapture.Add(VictimDefenceDef);
}

float UDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float Defence = 0.f;
	GetCapturedAttributeMagnitude(VictimDefenceDef, Spec, EvaluationParameters, Defence);

	float Strength = 0.f;
	GetCapturedAttributeMagnitude(AttackerStrengthDef, Spec, EvaluationParameters, Strength);

	const float Damage = Spec.GetSetByCallerMagnitude("Damage", false, Strength);
	const float Result = FMath::Max(Damage - FMath::Max(Defence, 0.f), 0.f);
	UE_LOG(LogTemp, Warning, TEXT("Dealing %f damage to the victim "), Result);
	return -Result;

	
}
