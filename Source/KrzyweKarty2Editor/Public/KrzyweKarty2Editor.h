#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class UKKGameplayAbility;
class UCharacterDataAsset;
class IAssetTypeActions;
class FAssetTypeActions_Base;

class FKrzyweKarty2EditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

protected:
    TArray<TSharedRef<IAssetTypeActions>> AssetTypeActions;

    void CreateCharacterSetup();

    template<typename T>
    T* CreateAsset(FString AssetPath, UClass* AssetClass, UFactory* AssetFactory);

    UCharacterDataAsset* CreateCharacterDataAsset(FString AssetPath);
    UBlueprint* CreateCharacterAbility(FString AssetPath);
    
public:
    void ExtendContentBrowser();

    TArray<FString> FolderPaths;
};
