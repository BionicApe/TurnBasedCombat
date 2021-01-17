// Created by Bionic Ape. All Rights Reserved.


#include "Combat/ActionType_TrpgFlee.h"
#include "TrpgCombatTypes.h"

#define LOCTEXT_NAMESPACE "UActionType_TrpgFlee"

UActionType_TrpgFlee::UActionType_TrpgFlee()
{
	bRequiresReceiver = false;
	bRequiresSender = true;
	bExecuteOnEverything = false;
	bExecuteOnSelf = true;
	bExecuteOnEnemy = false;
	bExecuteOnAlly = false;
	bRequiresSender = true;
	bRequiresReceiver = false;
}

bool UActionType_TrpgFlee::Validate(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const
{
	return true;
}

void UActionType_TrpgFlee::PerformAction(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const
{
	int32 const Random = FMath::FRandRange(0, 100);

/*	if (SuccessRate > Random)
	{
		Request.Sender->Flee();
		Result.Log.Messages.Add(
			FText::Format(
				LOCTEXT("UActionType_TrpgFlee", "{0} successfully fled the fight"),
				FText::FromString(Request.Sender->GetWarriorName())
			)
		);
	}
	else {
		Result.Log.Messages.Add(
			FText::Format(
				LOCTEXT("UActionType_TrpgFlee2", "{0} tried to flee"),
				FText::FromString(Request.Sender->GetWarriorName())
			)
		);
	}*/	
}

#undef LOCTEXT_NAMESPACE