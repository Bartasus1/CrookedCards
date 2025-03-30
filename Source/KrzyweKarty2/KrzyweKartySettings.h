// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"

#include "Engine/DeveloperSettings.h"
#include "GameplayTags.h"
#include "KrzyweKartySettings.generated.h"

class UCharacterWidget;
class UGameplayEffect;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Character Default Settings"))
class KRZYWEKARTY2_API UKrzyweKartySettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:

	UKrzyweKartySettings();

	static const UKrzyweKartySettings* Get()
	{
		return GetDefault<UKrzyweKartySettings>();
	}

	static const UGameplayEffect* GetAttackGameplayEffect()
	{
		return Get()->AttackGameplayEffectClass->GetDefaultObject<UGameplayEffect>();
	}
 
	UPROPERTY(EditAnywhere, Config)
	TSoftObjectPtr<UStaticMesh> CardMesh;

	UPROPERTY(EditAnywhere, Config)
	TSoftClassPtr<UCharacterWidget> CardWidgetClass;

	UPROPERTY(EditAnywhere, Config)
	TSubclassOf<UGameplayEffect> AttackGameplayEffectClass;

	UPROPERTY(EditAnywhere, Config, meta=(ForceInlineRow))
	TMap<FGameplayTag, FText> FractionTagToNameMap;
};
