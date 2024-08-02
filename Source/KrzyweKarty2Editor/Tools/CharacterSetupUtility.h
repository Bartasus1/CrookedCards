// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/AssetActionUtility.h"
#include "CharacterSetupUtility.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY2EDITOR_API UCharacterSetupUtility : public UAssetActionUtility
{
	GENERATED_BODY()

protected:

	UObject* CreateAsset(FString AssetPath, UClass* AssetClass);
};
