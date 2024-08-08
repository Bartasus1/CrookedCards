#include "KrzyweKarty2Editor.h"
#include "ContentBrowserModule.h"
#include "EdGraphUtilities.h"
#include "GameplayAbilitiesBlueprintFactory.h"
#include "GameplayAbilitiesBlueprintFactory.h"

#include "AssetRegistry/AssetRegistryModule.h"

#include "CharacterSlotStatus/CharacterSlotStatusFactory.h"

#include "CustomPins/BoardCoordinateGraphPinFactory.h"

#include "Factories/DataAssetFactory.h"

#include "Kismet2/KismetEditorUtilities.h"

#include "KrzyweKarty2/AbilitySystem/Abilities/KKGameplayAbility.h"
#include "KrzyweKarty2/Characters/CharacterDataAsset.h"

#define LOCTEXT_NAMESPACE "FKrzyweKarty2EditorModule"

void FKrzyweKarty2EditorModule::StartupModule()
{
    AssetTypeActions.Add(MakeShared<FCharacterSlotStatusAssetTypeActions>());
    
    IAssetTools& AssetToolsModule = FAssetToolsModule::GetModule().Get();
    for (auto AssetTypeAction : AssetTypeActions)
    {
        AssetToolsModule.RegisterAssetTypeActions(AssetTypeAction);
    }

    // CUSTOM PINS //
    const TSharedPtr<FBoardCoordinateGraphPinFactory> BoardCoordinateGraphPinFactoryGraphPinFactory = MakeShareable(new FBoardCoordinateGraphPinFactory());
    FEdGraphUtilities::RegisterVisualPinFactory(BoardCoordinateGraphPinFactoryGraphPinFactory);

    ExtendContentBrowser();
}

void FKrzyweKarty2EditorModule::ShutdownModule()
{
    
}

void FKrzyweKarty2EditorModule::CreateCharacterSetup()
{
    for (FString& Path : FolderPaths)
    {
        FString FolderName = FPaths::GetBaseFilename(Path);
        Path = Path + "/" + FolderName;
        
        UCharacterDataAsset* CharacterDataAsset = CreateCharacterDataAsset(Path);
        UBlueprint* CharacterAbility = CreateCharacterAbility(Path);

        if(CharacterDataAsset && CharacterAbility)
        {
            CharacterDataAsset->CharacterAbilityClass = CharacterAbility->GeneratedClass;
        }
    }
    
}

UCharacterDataAsset* FKrzyweKarty2EditorModule::CreateCharacterDataAsset(FString AssetPath)
{
    AssetPath += "_DataAsset";

    UDataAssetFactory* DataAssetFactory = NewObject<UDataAssetFactory>();

    UCharacterDataAsset* CharacterDataAsset = CreateAsset<UCharacterDataAsset>(AssetPath, UCharacterDataAsset::StaticClass(), DataAssetFactory);
    return CharacterDataAsset;
}

UBlueprint* FKrzyweKarty2EditorModule::CreateCharacterAbility(FString AssetPath)
{
    AssetPath += "_Ability";

    if(StaticLoadObject(UObject::StaticClass(), nullptr, *AssetPath))
    {
        return nullptr;
    }

    if(!FKismetEditorUtilities::CanCreateBlueprintOfClass(UKKGameplayAbility::StaticClass()))
    {
        return nullptr;
    }

    if(UPackage* Package = CreatePackage(*AssetPath))
    {
        UBlueprint* Blueprint = FKismetEditorUtilities::CreateBlueprint(UKKGameplayAbility::StaticClass(), Package, *FPaths::GetBaseFilename(AssetPath), BPTYPE_Normal);
        
        FAssetRegistryModule::AssetCreated(Blueprint);
        Blueprint->MarkPackageDirty();

        return Blueprint;
    }

    return nullptr;
}

template<typename T>
T* FKrzyweKarty2EditorModule::CreateAsset(FString AssetPath, UClass* AssetClass, UFactory* AssetFactory)
{
    IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

    UObject* Asset = AssetTools.CreateAsset(FPaths::GetBaseFilename(AssetPath), FPaths::GetPath(AssetPath), AssetClass, AssetFactory);
    return Cast<T>(Asset);
}

void FKrzyweKarty2EditorModule::ExtendContentBrowser()
{
    FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

    //ContentBrowserModule.GetAllPathViewContextMenuExtenders().Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FKrzyweKarty2EditorModule::CreateMenuExtender));
    ContentBrowserModule.GetAllPathViewContextMenuExtenders().Add(
        FContentBrowserMenuExtender_SelectedPaths::CreateLambda([this](const TArray<FString>& SelectedPaths) -> TSharedRef<FExtender>
        {
            TSharedRef<FExtender> MenuExtender(new FExtender());
            MenuExtender->AddMenuExtension
            (
                "Explore",
                EExtensionHook::After,
                TSharedPtr<FUICommandList>(),
                FMenuExtensionDelegate::CreateLambda([this](FMenuBuilder& MenuBuilder)
                {
                    MenuBuilder.AddMenuEntry
                    (
                        TAttribute(FText::FromString("Create Character Assets")),
                        TAttribute(FText::FromString("Create all assets what will be needed for proper character creation")),
                        FSlateIcon(),
                        FExecuteAction::CreateRaw(this, &FKrzyweKarty2EditorModule::CreateCharacterSetup)
                    );
                })
            );

            FolderPaths = SelectedPaths;

            return MenuExtender;
        })
    );
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FKrzyweKarty2EditorModule, KrzyweKarty2Editor)