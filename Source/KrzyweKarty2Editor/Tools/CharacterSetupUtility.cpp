// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSetupUtility.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"

UObject* UCharacterSetupUtility::CreateAsset(FString AssetPath, UClass* AssetClass)
{
	IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
	
	UFactory* Factory = nullptr;
	for (UFactory* Fac : AssetTools.GetNewAssetFactories())
	{
		if(Fac->SupportedClass == AssetClass)
		{
			Factory = Fac;
			break;
		}
	}

	if(Factory == nullptr || Factory->SupportedClass != AssetClass)
	{
		return nullptr;
	}

	UObject* Asset = AssetTools.CreateAsset(FPaths::GetBaseFilename(AssetPath), FPaths::GetPath(AssetPath), AssetClass, Factory);
	return Asset;
}
