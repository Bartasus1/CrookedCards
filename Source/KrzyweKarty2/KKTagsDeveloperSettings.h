// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Engine/DeveloperSettings.h"
#include "KKTagsDeveloperSettings.generated.h"

/**
 * 
 */

USTRUCT()
struct FImportantTags
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag CharacterActions;

};

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Krzywe Karty Tags Default Settings"))
class KRZYWEKARTY2_API UKKTagsDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	static const UKKTagsDeveloperSettings* Get()
	{
		return GetDefault<UKKTagsDeveloperSettings>();
	}

	UPROPERTY(EditAnywhere, Config)
	FImportantTags ImportantTags;
};
