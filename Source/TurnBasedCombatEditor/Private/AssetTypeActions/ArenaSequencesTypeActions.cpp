// Created by Bionic Ape. All Rights Reserved.

#include "ArenaSequencesTypeActions.h"
#include "Arena/ArenaSequences.h"

#define LOCTEXT_NAMESPACE "ArenaSequences_TypeActions"

FArenaSequencesTypeActions::FArenaSequencesTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FArenaSequencesTypeActions::GetName() const
{
	return LOCTEXT("FArenaSequencesTypeActionsName", "Arena Sequences");
}

FColor FArenaSequencesTypeActions::GetTypeColor() const
{
	return FColor::Yellow;
}

UClass* FArenaSequencesTypeActions::GetSupportedClass() const
{
	return UArenaSequences::StaticClass();
}

uint32 FArenaSequencesTypeActions::GetCategories()
{
	return AssetCategory;
}

#undef LOCTEXT_NAMESPACE