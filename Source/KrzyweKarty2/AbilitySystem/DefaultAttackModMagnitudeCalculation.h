

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "DefaultAttackModMagnitudeCalculation.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY2_API UDefaultAttackModMagnitudeCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

	UDefaultAttackModMagnitudeCalculation();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
private:
	FGameplayEffectAttributeCaptureDefinition TargetHealthCapture;
	FGameplayEffectAttributeCaptureDefinition TargetDefenceCapture;

	FGameplayEffectAttributeCaptureDefinition SourceStrengthCapture;
};
