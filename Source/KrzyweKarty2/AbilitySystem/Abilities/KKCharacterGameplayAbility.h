// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KKGameplayAbility.h"
#include "KKCharacterGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY2_API UKKCharacterGameplayAbility : public UKKGameplayAbility // class to use for character abilities
{
	GENERATED_BODY()
public:
	UKKCharacterGameplayAbility();
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
