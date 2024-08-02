// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "KKGameplayAbility.generated.h"

class ACharacterSlot;
class AKKGameBoard;
class AKKGameState;
class AKKCharacter;
/**
 * 
 */
UCLASS()
class KRZYWEKARTY2_API UKKGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	uint8 AbilityActionWeight;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	//Inspired by Xist XCL_ClientServerAbility (https://www.youtube.com/watch?v=VvkIuUnk05M)
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Activate Local Player Ability")
	void K2_ActivateLocalPlayerAbility();

	UFUNCTION(BlueprintImplementableEvent, DisplayName="Activate Ability With TargetData")
	void K2_ActivateAbilityWithTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle, FGameplayTag ApplicationTag);

	UFUNCTION(BlueprintCallable)
	virtual void NotifyTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetDataHandle, FGameplayTag ApplicationTag);

	virtual void ActivateServerAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	AKKCharacter* SourceCharacter;

	UPROPERTY(BlueprintReadOnly)
	AKKGameState* GameState;

	UPROPERTY(BlueprintReadOnly)
	AKKGameBoard* GameBoard;

	FDelegateHandle OnNotifyTargetDataReady;

	UPROPERTY(BlueprintReadWrite)
	TArray<ACharacterSlot*> ActionSlots;
};
