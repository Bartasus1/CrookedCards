// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "KKBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY2_API UKKBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	static FText GetFractionNameByTag(const FGameplayTag& FractionTag);
};
