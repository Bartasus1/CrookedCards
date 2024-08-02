#include "DefaultAttackModMagnitudeCalculation.h"

#include "Attributes/KKAttributeSet.h"

UDefaultAttackModMagnitudeCalculation::UDefaultAttackModMagnitudeCalculation()
{
	TargetHealthCapture.AttributeToCapture = UKKAttributeSet::GetHealthAttribute();
	TargetHealthCapture.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	TargetHealthCapture.bSnapshot = false;

	TargetDefenceCapture.AttributeToCapture = UKKAttributeSet::GetDefenceAttribute();
	TargetDefenceCapture.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	TargetDefenceCapture.bSnapshot = false;

	SourceStrengthCapture.AttributeToCapture = UKKAttributeSet::GetStrengthAttribute();
	SourceStrengthCapture.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	SourceStrengthCapture.bSnapshot = false;

	RelevantAttributesToCapture.Add(TargetHealthCapture);
	RelevantAttributesToCapture.Add(TargetDefenceCapture);
	RelevantAttributesToCapture.Add(SourceStrengthCapture);
}

float UDefaultAttackModMagnitudeCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();;

	float TargetHealth = 0.f;
	float TargetDefence = 0.f;
	float SourceStrength = 0.f;

	GetCapturedAttributeMagnitude(TargetHealthCapture, Spec, EvaluationParameters, TargetHealth);
	GetCapturedAttributeMagnitude(TargetDefenceCapture, Spec, EvaluationParameters, TargetDefence);
	GetCapturedAttributeMagnitude(SourceStrengthCapture, Spec, EvaluationParameters, SourceStrength);

	const float DamageToDeal = FMath::Max(SourceStrength - TargetDefence, 0.f);

	return TargetHealth - DamageToDeal;
}
