// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "CharacterAttackExecCalculation.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY2_API UCharacterAttackExecCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UCharacterAttackExecCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defence);
};
