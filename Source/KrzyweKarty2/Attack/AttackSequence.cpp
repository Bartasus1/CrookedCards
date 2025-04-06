// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSequence.h"
#include "AttackComponent.h"
#include "ControlFlow.h"
#include "ControlFlowManager.h"
#include "KrzyweKarty2/KrzyweKartySettings.h"
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
	check(AttackContext.IsValid());
	
	GatherAttackComponents(GetAttacker());
	GatherAttackComponents(GetVictim());

	FControlFlow& Flow = FControlFlowStatics::Create(this, "AttackFlow")
		.QueueStep("PreAttack", this, &ThisClass::ExecuteAttackStage, EAttackStage::PreAttack)
		.QueueStep("InitDamage", this, &ThisClass::ModifyDamage, NULL, EGameplayModOp::Max)
		.QueueStep("Before", this, &ThisClass::ExecuteAttackStage, EAttackStage::Before)
		.QueueStep("ExecuteDamage", this, &ThisClass::ExecuteDamage)
		.QueueStep("After", this, &ThisClass::ExecuteAttackStage, EAttackStage::After)
		.QueueStep("CheckVictimDeath", this, &ThisClass::CheckVictimDeath);

	AttackFlow = Flow.AsShared();
	AttackFlow->ExecuteFlow();
}

void UAttackSequence::StopExecution()
{
	AttackFlow->CancelFlow();
}

void UAttackSequence::ModifyDamage(int32 InDamage, EGameplayModOp::Type ModificationType)
{
	switch (ModificationType)
	{
	case EGameplayModOp::Additive:
		Damage += InDamage;
		break;
	case EGameplayModOp::Override:
		Damage = InDamage;
		break;
	case EGameplayModOp::Multiplicitive:
		Damage *= InDamage;
		break;
	case EGameplayModOp::Division:
		Damage /= InDamage;
		break;
	case EGameplayModOp::Max:
		if(Damage == -1)
		{
			Damage = GetAttacker()->GetStrength(); // damage initialization, only if it was not set before
		}
		break;
	default:
		break;
	}
}


void UAttackSequence::ExecuteAttackStage(EAttackStage Stage)
{
	for (const UAttackComponent* AttackComponent : AttackPipeline[Stage])
	{
		AttackComponent->AttackStageExecution.Broadcast(this);
	}
}

void UAttackSequence::ExecuteDamage()
{
	const float Level = FMath::Max<int32>(AttackContext.AbilityIndex, 0);
	const UGameplayEffect* AttackGameplayEffect = UKrzyweKartySettings::GetAttackGameplayEffect();
	UAbilitySystemComponent* AbilitySystemComponent = GetAttacker()->GetAbilitySystemComponent();
	
	FGameplayEffectSpec Spec = FGameplayEffectSpec(AttackGameplayEffect, AbilitySystemComponent->MakeEffectContext(), Level);
	Spec.SetSetByCallerMagnitude("Damage", Damage);
	
	AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(Spec, GetVictim()->GetAbilitySystemComponent());
}

void UAttackSequence::CheckVictimDeath()
{
	if(AKKCharacter* Character = GetVictim(); Character->GetHealth() <= 0)
	{
		Character->OnCharacterDeath.Broadcast();
		Character->Destroy();
	}
}

void UAttackSequence::GatherAttackComponents(const AKKCharacter* Character)
{
	Character->ForEachComponent<UAttackComponent>(false, [this, &Character](UAttackComponent* AttackComponent)
	{
		if(DoesComponentMatchPrerequisites(AttackComponent, Character))
		{
			AttackPipeline[AttackComponent->AttackStage].Add(AttackComponent);
		}
	});
}

bool UAttackSequence::DoesComponentMatchPrerequisites(const UAttackComponent* AttackComponent, const AKKCharacter* Character) const
{
	return AttackComponent->MatchesAttackType(GetAttackType(), GetAbilityIndex()) && AttackContext.DoesCharacterMatchRole(Character, AttackComponent->AttackRole);
}
