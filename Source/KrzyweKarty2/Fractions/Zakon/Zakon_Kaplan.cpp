// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Kaplan.h"
#include "GameplayAbilities/Public/GameplayEffectExecutionCalculation.h"


// Sets default values
AZakon_Kaplan::AZakon_Kaplan()
{

}

void AZakon_Kaplan::Attacker_OnAttackEnd(AKKCharacter* TargetCharacter, const FAttackInfo& AttackInfo, FGameplayEffectCustomExecutionOutput& OutExecutionOutput)
{
	if(AttackInfo.AttackType == EAttackType::DefaultAttack)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UKKAttributeSet::GetManaAttribute(), EGameplayModOp::Additive, 3));
	}
}
