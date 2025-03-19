// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSequence.h"
#include "KrzyweKarty2/Characters/KKCharacter.h"

UAttackSequence::UAttackSequence(): AttackContext()
{
	for (EAttackStage AttackStage : TEnumRange<EAttackStage>())
	{
		AttackPipeline.Add(AttackStage, TArray<UAttackComponent*>());
	}
}

void UAttackSequence::BeginAttack()
{
	GetAttacker()->ForEachComponent<UAttackComponent>(false, [&](UAttackComponent* AttackComponent)
	{
		AttackPipeline[AttackComponent->AttackStage].Add(AttackComponent);
	});

	GetVictim()->ForEachComponent<UAttackComponent>(false, [&](UAttackComponent* AttackComponent)
	{
		AttackPipeline[AttackComponent->AttackStage].Add(AttackComponent);
	});

	//TODO: Add ability to interupt sequence & add attack result info
	ExecuteAttackStage(EAttackStage::PreAttack);
	ExecuteAttackStage(EAttackStage::Before);
	
	UGameplayEffect* GameplayEffect = AttackGameplayEffectClass->GetDefaultObject<UGameplayEffect>();
	GetAttacker()->GetAbilitySystemComponent()->ApplyGameplayEffectToTarget(GameplayEffect, GetVictim()->GetAbilitySystemComponent());

	ExecuteAttackStage(EAttackStage::After);
	
}

void UAttackSequence::ExecuteAttackStage(EAttackStage Stage)
{
	for (const UAttackComponent* AttackComponent : AttackPipeline[Stage])
	{
		AttackComponent->AttackStageExecution.Broadcast(this);
	}
}
