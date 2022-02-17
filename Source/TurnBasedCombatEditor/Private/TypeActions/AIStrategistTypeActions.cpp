// Created by Bionic Ape. All Rights Reserved.

#include "TypeActions/AIStrategistTypeActions.h"
#include "AIStrategist.h"

#define LOCTEXT_NAMESPACE "AIStrategist_TypeActions"

FAIStrategistTypeActions::FAIStrategistTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FAIStrategistTypeActions::GetName() const
{
	return LOCTEXT("FAIStrategistTypeActionsName", "AIStrategist");
}

FColor FAIStrategistTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FAIStrategistTypeActions::GetSupportedClass() const
{
	return UAIStrategist::StaticClass();
}

uint32 FAIStrategistTypeActions::GetCategories()
{
	return AssetCategory;
}

#undef LOCTEXT_NAMESPACE