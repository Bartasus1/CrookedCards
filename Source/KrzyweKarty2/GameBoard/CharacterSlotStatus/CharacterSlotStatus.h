// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "UObject/Object.h"
#include "CharacterSlotStatus.generated.h"

/**
 * 
 */

class ACharacterSlot;
class UCharacterSlotStatus;

UCLASS()
class KRZYWEKARTY2_API UCharacterSlotStatus : public UObject
{
	GENERATED_BODY()
public:
	UCharacterSlotStatus();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag StatusTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor StatusColor = FLinearColor(1.f, 1.f, 1.f, 0.7);
};
