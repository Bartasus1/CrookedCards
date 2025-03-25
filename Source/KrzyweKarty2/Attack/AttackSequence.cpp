// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSequence.h"
#include "ControlFlowManager.h"
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
	if(AttackFlow)
	{
		AttackFlow->CancelFlow();
		UE_LOG(LogTemp, Warning, TEXT("Attack canceled!"));
	}
}

void UAttackSequence::ModifyDamage(int32 InDamage, EGameplayModOp::Type ModificationType)
{
	//TODO: proper damage initialization & preparation for damage negotiation during attack
	const int32 Strength = GetAttacker()->GetStrength();
	switch (ModificationType)
	{
	case EGameplayModOp::Additive:
		Damage = Strength + InDamage;
		break;
	case EGameplayModOp::Override:
		Damage = InDamage;
		break;
	case EGameplayModOp::Multiplicitive:
		Damage = Strength * InDamage;
		break;
	case EGameplayModOp::Division:
		Damage = Strength / InDamage;
		break;
	default:
		Damage = Strength; // initialize damage
		break;
	}
}


void UAttackSequence::ExecuteAttackStage(EAttackStage Stage)
{
	for (const UAttackComponent* AttackComponent : AttackPipeline[Stage])
	{
		AttackComponent->AttackStageExecution.Broadcast(this);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Attack stage %s executed"), *UEnum::GetValueAsString(Stage));
}

void UAttackSequence::ExecuteDamage()
{
	const float Level = AttackContext.AbilityIndex;
	const UAbilitySystemComponent* AbilitySystemComponent = GetAttacker()->GetAbilitySystemComponent();
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(AttackGameplayEffectClass, Level, AbilitySystemComponent->MakeEffectContext());

	if(Damage.IsSet())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Attack.Damage"), Damage.Get(GetAttacker()->GetStrength()));
	}
	
	GetAttacker()->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetVictim()->GetAbilitySystemComponent());
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
	//TODO: add ability index check (maybe make it another class if it gets too messy)
	return AttackComponent->AttackType == GetAttackType() && AttackContext.DoesCharacterMatchRole(Character, AttackComponent->AttackRole);
}
