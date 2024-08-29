// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitTargetDataUnderCursor.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTargetDataReady, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

class AKKPlayerController;

UCLASS()
class KRZYWEKARTY2_API UAbilityTask_WaitTargetDataUnderCursor : public UAbilityTask
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta=(DisplayName = "Wait Target Data Under Cursor", DefaultToSelf = "OwningAbility", HidePin = "OwningAbility", BlueprintInternalUseOnly))
	static UAbilityTask_WaitTargetDataUnderCursor* CreateTargetDateUnderCursor(UGameplayAbility* OwningAbility, ECollisionChannel CollisionChannel, FGameplayTag ConfirmInputTag, FGameplayTag CancelInputTag);


	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
protected:

	// INPUT
	UPROPERTY()
	TEnumAsByte<ECollisionChannel> CollisionChannel;
	
	UPROPERTY()
	FGameplayTag ConfirmInputTag;

	UPROPERTY()
	FGameplayTag CancelInputTag;
	
	// OUTPUT 
	UPROPERTY(BlueprintAssignable)
	FOnAbilityTargetDataReady ConfirmedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityTargetDataReady CanceledDelegate;

private:

	UPROPERTY()
	AKKPlayerController* PlayerController;
	
	void ConfirmTargeting() const;
	void CancelTargeting() const;

	UFUNCTION()
	void ProcessInput(FGameplayTag InInputTag);
	
};
