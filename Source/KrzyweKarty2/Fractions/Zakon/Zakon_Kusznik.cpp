// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Kusznik.h"
#include "GameplayAbilities/Public/GameplayEffectExecutionCalculation.h"

// Sets default values
AZakon_Kusznik::AZakon_Kusznik()
{

}

void AZakon_Kusznik::Attacker_OnAttackEnd(AKKCharacter* TargetCharacter, const FAttackInfo& AttackInfo, FGameplayEffectCustomExecutionOutput& OutExecutionOutput)
{
	if(AttackInfo.AttackType == EAttackType::DefaultAttack)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UKKAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, 2.f));
	}
}


