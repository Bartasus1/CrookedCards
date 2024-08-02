// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "KKCharacterDeveloperSettings.generated.h"

class UCharacterWidget;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Character Default Settings"))
class KRZYWEKARTY2_API UKKCharacterDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:

	UKKCharacterDeveloperSettings();

	static const UKKCharacterDeveloperSettings* Get()
	{
		return GetDefault<UKKCharacterDeveloperSettings>();
	}
 
	UPROPERTY(EditAnywhere, Config)
	TSoftObjectPtr<UStaticMesh> CardMesh;

	UPROPERTY(EditAnywhere, Config)
	TSoftClassPtr<UCharacterWidget> CardWidgetClass;
};
