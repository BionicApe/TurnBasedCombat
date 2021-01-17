// Created by Bionic Ape. All rights reseved.

#include "TeamAssetTypeAction.h"
// Runtime Module
#include "Team.h"

#define LOCTEXT_NAMESPACE "BAMultiplayer_Team"


FTeamAssetTypeAction::FTeamAssetTypeAction(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FTeamAssetTypeAction::GetName() const
{
	return LOCTEXT("FTeamAssetTypeActionName", "Team");
}

FColor FTeamAssetTypeAction::GetTypeColor() const
{
	return FColor::Purple;
}

UClass* FTeamAssetTypeAction::GetSupportedClass() const
{
	return UTeam::StaticClass();
}

uint32 FTeamAssetTypeAction::GetCategories()
{
	return AssetCategory;
}

#undef LOCTEXT_NAMESPACE