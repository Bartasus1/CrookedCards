// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameplayAbility.h"

#include "Kismet/KismetSystemLibrary.h"

#include "KrzyweKarty2/KKTagsDeveloperSettings.h"
#include "KrzyweKarty2/Characters/KKCharacter.h"
#include "KrzyweKarty2/Core/KKGameState.h"
#include "KrzyweKarty2/GameBoard/CharacterSlot.h"

bool UKKGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bCanActivate = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	
	if(AbilityActionWeight == 0)
	{
		return bCanActivate;
	}
	
	if(const AKKCharacter* Character = Cast<AKKCharacter>(ActorInfo->AvatarActor))
	{
		if(Character->CharacterActions >= AbilityActionWeight)
		{
			OptionalRelevantTags->AddTag(UKKTagsDeveloperSettings::Get()->ImportantTags.CharacterActions);

			return false;
		}
	}

	return bCanActivate;
}

void UKKGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// initialize member variables
	SourceCharacter = Cast<AKKCharacter>(GetAvatarActorFromActorInfo());
	GameState = GetWorld()->GetGameState<AKKGameState>();
	GameBoard = GameState->GetGameBoard();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(ActorInfo->IsLocallyControlled())
	{
		K2_ActivateLocalPlayerAbility();
	}
	else if(ActorInfo->IsNetAuthority())
	{
		ActivateServerAbility(Handle, ActorInfo, ActivationInfo);
	}
}

void UKKGameplayAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);

	//SourceCharacter->CharacterActions = AbilityActionWeight;
}

void UKKGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if(UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->AbilityTargetDataSetDelegate(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey()).Remove(OnNotifyTargetDataReady);
		ASC->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());
	}
	
	if(ActorInfo->IsLocallyControlled())
	{
		for (ACharacterSlot* CharacterSlot : ActionSlots)
		{
			//UKismetSystemLibrary::PrintString(this, CharacterSlot->GetName());
			CharacterSlot->SetLocalStatus(nullptr);
		}

		ActionSlots.Empty();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UKKGameplayAbility::NotifyTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetDataHandle, FGameplayTag ApplicationTag)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent);
	
	if(CurrentActorInfo->IsLocallyControlled() && !CurrentActorInfo->IsNetAuthority())
	{
		AbilitySystemComponent->CallServerSetReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey(), TargetDataHandle, ApplicationTag, AbilitySystemComponent->ScopedPredictionKey);
	}

	K2_ActivateAbilityWithTargetData(TargetDataHandle, ApplicationTag);

	AbilitySystemComponent->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());
}

void UKKGameplayAbility::ActivateServerAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	OnNotifyTargetDataReady = ASC->AbilityTargetDataSetDelegate(Handle, ActivationInfo.GetActivationPredictionKey()).AddUObject(this, &UKKGameplayAbility::NotifyTargetDataReady);
}
