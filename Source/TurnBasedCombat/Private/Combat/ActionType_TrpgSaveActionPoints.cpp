// Created by Bionic Ape. All Rights Reserved.


#include "Combat/ActionType_TrpgSaveActionPoints.h"
#include "TrpgCombatTypes.h"
//#include "Combat/TurnModifier_SaveActionPoints.h"
//#include "Combat/TrpgFight.h"

#define LOCTEXT_NAMESPACE "UActionType_TrpgSaveActionPoints"

UActionType_TrpgSaveActionPoints::UActionType_TrpgSaveActionPoints()
{
	ActionPoints = 0;
}

void UActionType_TrpgSaveActionPoints::PerformAction(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const
{
	//UTurnModifier_SaveActionPoints* SaveActionPointsModifier = NewObject<UTurnModifier_SaveActionPoints>(Request.Sender->GetOuter());
	//int32 const PointsToSave = Request.Sender->CurrentActionPoints;
	//SaveActionPointsModifier->SavedActionPoints = PointsToSave;
	//Request.Sender->TurnModifiers.Add(SaveActionPointsModifier);
	//
	//Result.Log.Messages.Add(
	//	FText::Format(
	//		LOCTEXT("UActionType_TrpgSaveActionPoints", "{0} Successfully saved {1} for the next turn"),
	//		FText::FromString(Request.Sender->GetWarriorName()),
	//		FText::FromString(FString::FromInt(PointsToSave))
	//	)
	//);

	//Request.Sender->Fight->FinishWarriorsTurn(Request.Sender);
}

#undef LOCTEXT_NAMESPACE