// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_WaitTargetDataUnderCursor.h"
#include "AbilitySystemComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "KrzyweKarty2/Core/KKPlayerController.h"

UAbilityTask_WaitTargetDataUnderCursor* UAbilityTask_WaitTargetDataUnderCursor::CreateTargetDateUnderCursor(UGameplayAbility* OwningAbility, ECollisionChannel CollisionChannel, FGameplayTag ConfirmInputTag, FGameplayTag CancelInputTag)
{
	UAbilityTask_WaitTargetDataUnderCursor* MyObj = NewAbilityTask<UAbilityTask_WaitTargetDataUnderCursor>(OwningAbility);

	MyObj->CollisionChannel = CollisionChannel;
	MyObj->ConfirmInputTag = ConfirmInputTag;
	MyObj->CancelInputTag = CancelInputTag;

	return MyObj;
}

void UAbilityTask_WaitTargetDataUnderCursor::Activate()
{
	Super::Activate();

	PlayerController = Cast<AKKPlayerController>(Ability->GetCurrentActorInfo()->PlayerController.Get());
	if(PlayerController)
	{
		PlayerController->ToggleConfirmationState(true);
		PlayerController->OnInputActivated.AddDynamic(this, &UAbilityTask_WaitTargetDataUnderCursor::ProcessInput);
	}
}

void UAbilityTask_WaitTargetDataUnderCursor::ConfirmTargeting() const
{
	FHitResult HitResult; 
	PlayerController->GetHitResultUnderCursor(CollisionChannel, false, HitResult);
	
	FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);

	const FGameplayAbilityTargetDataHandle TargetDataHandle(TargetData);
	
	ConfirmedDelegate.Broadcast(TargetDataHandle);
}

void UAbilityTask_WaitTargetDataUnderCursor::CancelTargeting() const
{
	const FGameplayAbilityTargetDataHandle TargetDataHandle;
	CanceledDelegate.Broadcast(TargetDataHandle);
}

void UAbilityTask_WaitTargetDataUnderCursor::ProcessInput(FGameplayTag InInputTag)
{
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		if(InInputTag == ConfirmInputTag)
		{
			ConfirmTargeting();
		}
		if(InInputTag == CancelInputTag)
		{
			CancelTargeting();
		}

		PlayerController->ToggleConfirmationState(false);
	}
}
