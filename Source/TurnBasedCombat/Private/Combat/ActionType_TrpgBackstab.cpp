// Created by Bionic Ape. All Rights Reserved.


#include "Combat/ActionType_TrpgBackstab.h"
#include "TrpgCombatTypes.h"

#define LOCTEXT_NAMESPACE "UActionType_TrpgProvoke"

void UActionType_TrpgBackstab::PerformAction(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const
{
	//Result.Log.Messages.Add(
	//	FText::Format(
	//		LOCTEXT("UActionType_TrpgProvoke", "{0} tried to backstab {1}"),
	//		FText::FromString(Request.Sender->GetWarriorName()),
	//		FText::FromString(Request.Receiver->GetWarriorName())
	//	)
	//);
}

#undef LOCTEXT_NAMESPACE