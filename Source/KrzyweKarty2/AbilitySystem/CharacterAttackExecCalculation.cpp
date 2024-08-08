// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttackExecCalculation.h"
#include "AbilitySystemComponent.h"

#include "Attributes/KKAttributeSet.h"

#include "KrzyweKarty2/Characters/KKCharacter.h"

void UCharacterAttackExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	const float Health = TargetASC->GetNumericAttribute(UKKAttributeSet::GetHealthAttribute());

	if(Health <= 0.f)
	{
		if(AKKCharacter* Character = Cast<AKKCharacter>(TargetASC->GetOwnerActor()))
		{
			Character->OnCharacterDeath.Broadcast();
            Character->Destroy();
		}
	}
}
