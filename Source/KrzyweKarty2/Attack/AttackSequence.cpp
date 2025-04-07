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
	
	AssembleAttackPipeline(GetAttacker());
	AssembleAttackPipeline(GetVictim());

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
	const float Level = AttackContext.AbilityIndex.Get(0);
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

void UAttackSequence::AssembleAttackPipeline(const AKKCharacter* Character)
{
	Character->ForEachComponent<UAttackComponent>(false, [this, &Character](UAttackComponent* AttackComponent)
	{
		if(AttackContext.DoesCharacterMatchRole(Character, AttackComponent->AttackRole) &&
			AttackComponent->MatchesAttackType(AttackContext.AttackType, AttackContext.AbilityIndex))
		{
			AttackPipeline[AttackComponent->AttackStage].Add(AttackComponent);
		}
	});
}


DEFINE_FUNCTION(UAttackSequence::execBeginDefaultAttackSequence)
{
	P_GET_OBJECT(UObject, WorldContextObject);
	P_GET_OBJECT(AKKCharacter, Attacker);
	P_GET_OBJECT(AKKCharacter, Victim);
	P_FINISH;
	
	P_NATIVE_BEGIN;
	const FAttackContext AttackContext = FAttackContext(Attacker, Victim, EAttackType::DefaultAttack);
	
	UAttackSequence* AttackSequence = NewObject<UAttackSequence>(WorldContextObject);
	AttackSequence->AttackContext = AttackContext;
	AttackSequence->BeginAttack();

	*static_cast<UAttackSequence**>(RESULT_PARAM) = AttackSequence;
	P_NATIVE_END;
}

DEFINE_FUNCTION(UAttackSequence::execBeginAbilityAttackSequence)
{
	P_GET_OBJECT(UObject, WorldContextObject);
	P_GET_OBJECT(AKKCharacter, Attacker);
	P_GET_OBJECT(AKKCharacter, Victim);
	PARAM_PASSED_BY_VAL(InAbilityIndex, FIntProperty, int32);
	PARAM_PASSED_BY_VAL(InDamage, FIntProperty, int32);
	P_FINISH;
	
	P_NATIVE_BEGIN;
	const FAttackContext AttackContext = FAttackContext(Attacker, Victim, EAttackType::Ability, InAbilityIndex);
	
	UAttackSequence* AttackSequence = NewObject<UAttackSequence>(WorldContextObject);
	AttackSequence->AttackContext = AttackContext;
	AttackSequence->Damage = InDamage;
	AttackSequence->BeginAttack();

	*static_cast<UAttackSequence**>(RESULT_PARAM) = AttackSequence;
	P_NATIVE_END;
}
