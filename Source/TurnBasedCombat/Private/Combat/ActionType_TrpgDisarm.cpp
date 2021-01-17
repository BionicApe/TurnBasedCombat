// Created by Bionic Ape. All Rights Reserved.


#include "Combat/ActionType_TrpgDisarm.h"
#include "TrpgCombatTypes.h"


#define LOCTEXT_NAMESPACE "UActionType_TrpgSaveActionPoints"

void UActionType_TrpgDisarm::PerformAction(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const
{
	//Result.Log.Messages.Add(
	//	FText::Format(
	//		LOCTEXT("UActionType_TrpgSaveActionPoints", "{0} couldn't disarm {1}"),
	//		FText::FromString(Request.Sender->GetWarriorName()),
	//		FText::FromString(Request.Receiver->GetWarriorName())
	//	)
	//);
}
#undef LOCTEXT_NAMESPACE