// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Lucznik.h"


// Sets default values
AZakon_Lucznik::AZakon_Lucznik()
{
}

void AZakon_Lucznik::Attacker_OnAttackEnd(AKKCharacter* TargetCharacter, const FAttackInfo& AttackInfo, FGameplayEffectCustomExecutionOutput& OutExecutionOutput)
{
	AttackCounter++;

	if(AttackCounter >= 2)
	{
		TargetCharacter->AbilitySystemComponent->ApplyModToAttribute(UKKAttributeSet::GetDefenceAttribute(), EGameplayModOp::Additive, -1.f);
		AttackCounter = 0;
	}
}

