// Created by Bionic Ape. All Rights Reserved.

#include "TypeActions/TurnBasedCombatConfigTypeActions.h"
#include "Config/TurnBasedCombatConfig.h"

#define LOCTEXT_NAMESPACE "TurnBasedCombatConfig_TypeActions"

FTurnBasedCombatConfigTypeActions::FTurnBasedCombatConfigTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FTurnBasedCombatConfigTypeActions::GetName() const
{
	return LOCTEXT("FTurnBasedCombatConfigTypeActionsName", "TurnBasedCombatConfig");
}

FColor FTurnBasedCombatConfigTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FTurnBasedCombatConfigTypeActions::GetSupportedClass() const
{
	return UTurnBasedCombatConfig::StaticClass();
}

uint32 FTurnBasedCombatConfigTypeActions::GetCategories()
{
	return AssetCategory;
}

#undef LOCTEXT_NAMESPACE