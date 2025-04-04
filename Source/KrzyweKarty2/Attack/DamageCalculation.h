// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "DamageCalculation.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY2_API UDamageCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:

	UDamageCalculation();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:

	FGameplayEffectAttributeCaptureDefinition AttackerStrengthDef;

	FGameplayEffectAttributeCaptureDefinition VictimDefenceDef;
};
