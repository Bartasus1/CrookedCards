// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KrzyweKarty2/Characters/KKCharacter.h"
#include "Zakon_Kaplan.generated.h"

UCLASS()
class KRZYWEKARTY2_API AZakon_Kaplan : public AKKCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AZakon_Kaplan();


	virtual void Attacker_OnAttackEnd(AKKCharacter* TargetCharacter, const FAttackInfo& AttackInfo, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) override;
};
