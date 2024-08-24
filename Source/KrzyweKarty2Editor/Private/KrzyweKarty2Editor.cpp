#include "KrzyweKarty2Editor.h"
#include "EdGraphUtilities.h"

#include "CharacterAssetsTools/CharacterAssetsCreationTool.h"
#include "CharacterSlotStatus/CharacterSlotStatusFactory.h"
#include "CustomPins/BoardCoordinateGraphPinFactory.h"


#define LOCTEXT_NAMESPACE "FKrzyweKarty2EditorModule"

void FKrzyweKarty2EditorModule::StartupModule()
{
	// ASSET ACTIONS //
    AssetTypeActions.Add(MakeShared<FCharacterSlotStatusAssetTypeActions>());
    
    IAssetTools& AssetToolsModule = FAssetToolsModule::GetModule().Get();
    for (auto AssetTypeAction : AssetTypeActions)
    {
        AssetToolsModule.RegisterAssetTypeActions(AssetTypeAction);
    }

    // CUSTOM PINS //
    const TSharedPtr<FBoardCoordinateGraphPinFactory> BoardCoordinateGraphPinFactoryGraphPinFactory = MakeShareable(new FBoardCoordinateGraphPinFactory());
    FEdGraphUtilities::RegisterVisualPinFactory(BoardCoordinateGraphPinFactoryGraphPinFactory);

	// TOOLS //
	FCharacterAssetsCreationTool();
}

void FKrzyweKarty2EditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FKrzyweKarty2EditorModule, KrzyweKarty2Editor)