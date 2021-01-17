// Created by Bionic Ape. All Rights Reserved.


#include "Combat/ActionType_TrpgTaunt.h"
//#include "Combat/TurnModifier_Taunted.h"
#include "TrpgCombatTypes.h"

#define LOCTEXT_NAMESPACE "UActionType_TrpgTaunt"

void UActionType_TrpgTaunt::PerformAction(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const
{
	//UTurnModifier_Provoked* TauntedModifier = NewObject<UTurnModifier_Provoked>(Request.Receiver->GetOuter());
	//Request.Receiver->TurnModifiers.Add(TauntedModifier);
	//Result.Log.Messages.Add(
	//	FText::Format(
	//		LOCTEXT("UActionType_TrpgTaunt", "{0} has succesfully taunted {1}"),
	//		FText::FromString(Request.Sender->GetWarriorName()),
	//		FText::FromString(Request.Receiver->GetWarriorName())
	//	)
	//);
}
#undef LOCTEXT_NAMESPACE