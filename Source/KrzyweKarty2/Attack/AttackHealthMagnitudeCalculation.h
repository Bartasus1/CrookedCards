// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "AttackHealthMagnitudeCalculation.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY2_API UAttackHealthMagnitudeCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:

	UAttackHealthMagnitudeCalculation();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:

	FGameplayEffectAttributeCaptureDefinition AttackerStrengthDef;
	
	FGameplayEffectAttributeCaptureDefinition VictimHealthDef;
	FGameplayEffectAttributeCaptureDefinition VictimDefenceDef;
};
