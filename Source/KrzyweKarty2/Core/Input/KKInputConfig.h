// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Engine/DataAsset.h"
#include "KKInputConfig.generated.h"

/**
 * 
 */

class UInputMappingContext;
class UInputAction;

USTRUCT()
struct FInputActionTag
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UInputAction* InputAction;

	UPROPERTY(EditAnywhere)
	FGameplayTag InputTag = FGameplayTag::EmptyTag;
};

UCLASS()
class KRZYWEKARTY2_API UKKInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<FInputActionTag> InputConfig;

	UPROPERTY(EditAnywhere)
	UInputMappingContext* ConfirmationMappingContext;
};
