// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TurnBasedCombatEditor.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "TypeActions/TeamAssetTypeAction.h"
#include "TypeActions/ArenaSequencesTypeActions.h"
#include "Templates/SharedPointer.h"
#include "TypeActions/TurnBasedCombatConfigTypeActions.h"
#include "TypeActions/AIStrategistTypeActions.h"

#define LOCTEXT_NAMESPACE "FTurnBasedCombatEditorModule"

void FTurnBasedCombatEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type AssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("TurnBasedCombatEditor")), LOCTEXT("TurnBasedCombatEditor", "TurnBasedCombatEditor"));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FTeamAssetTypeAction(AssetCategoryBit)));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FArenaSequencesTypeActions(AssetCategoryBit)));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FTurnBasedCombatConfigTypeActions(AssetCategoryBit)));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FAIStrategistTypeActions(AssetCategoryBit)));
}

void FTurnBasedCombatEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTurnBasedCombatEditorModule, TurnBasedCombatEditor)