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
	GatherAttackComponents(GetAttacker());
	GatherAttackComponents(GetVictim());

	FControlFlow& Flow = FControlFlowStatics::Create(this, TEXT("AttackFlow"))
		.QueueStep("PreAttack", this, &ThisClass::ExecuteAttackStage, EAttackStage::PreAttack)
		.QueueStep("Before", this, &ThisClass::ExecuteAttackStage, EAttackStage::Before)
		.QueueStep("ExecuteDamage", this, &ThisClass::ExecuteDamage)
		.QueueStep("PreAttack", this, &ThisClass::ExecuteAttackStage, EAttackStage::After);

	AttackFlow = Flow.AsShared();
	
	Flow.ExecuteFlow();
}

void UAttackSequence::StopExecution()
{
	if(AttackFlow)
	{
		AttackFlow->CancelFlow();
		UE_LOG(LogTemp, Warning, TEXT("Cancel Flow Properly"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Cancel Flow"));
}

void UAttackSequence::ExecuteAttackStage(FControlFlowNodeRef FlowHandle, EAttackStage Stage)
{
	for (const UAttackComponent* AttackComponent : AttackPipeline[Stage])
	{
		AttackComponent->AttackStageExecution.Broadcast(this);
	}
	
	if(!FlowHandle->HasCancelBeenRequested())
	{
		FlowHandle->ContinueFlow();
	}
	

	UE_LOG(LogTemp, Warning, TEXT("Attack stage %s executed"), *UEnum::GetValueAsString(Stage));
}

void UAttackSequence::ExecuteDamage(FControlFlowNodeRef FlowHandle)
{
	UGameplayEffect* GameplayEffect = AttackGameplayEffectClass->GetDefaultObject<UGameplayEffect>();
	GetAttacker()->GetAbilitySystemComponent()->ApplyGameplayEffectToTarget(GameplayEffect, GetVictim()->GetAbilitySystemComponent());

	if(!FlowHandle->HasCancelBeenRequested())
	{
		FlowHandle->ContinueFlow();
	}
}

void UAttackSequence::GatherAttackComponents(const AKKCharacter* Character)
{
	Character->ForEachComponent<UAttackComponent>(false, [this, &Character](UAttackComponent* AttackComponent)
	{
		if(AttackContext.DoesCharacterMatchRole(Character, AttackComponent->AttackRole))
		{
			AttackPipeline[AttackComponent->AttackStage].Add(AttackComponent);
		}
	});
}
