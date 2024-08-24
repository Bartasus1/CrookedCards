// Fill out your copyright notice in the Description page of Project Settings.


#include "KKBlueprintFunctionLibrary.h"
#include "KKCharacterDeveloperSettings.h"

FText UKKBlueprintFunctionLibrary::GetFractionNameByTag(const FGameplayTag& FractionTag)
{
	const UKKCharacterDeveloperSettings* DeveloperSettings = UKKCharacterDeveloperSettings::Get();

	return DeveloperSettings->FractionTagToNameMap.FindRef(FractionTag);
}
