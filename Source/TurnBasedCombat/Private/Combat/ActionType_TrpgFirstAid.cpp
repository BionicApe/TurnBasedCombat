// Created by Bionic Ape. All Rights Reserved.


#include "Combat/ActionType_TrpgFirstAid.h"
#include "Containers/EnumAsByte.h"
#include "Components/HealthComponent.h"
#include "TrpgCombatTypes.h"


#define LOCTEXT_NAMESPACE "ActionType_TrpgFirstAid"

UActionType_TrpgFirstAid::UActionType_TrpgFirstAid() : Super()
{
	ActionTypeValue = (uint8)EActionTypeValue_Trpg::FIRST_AID;
}

void UActionType_TrpgFirstAid::PerformAction(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const
{
	//if (UHealthComponent* HealthComponent = Request.Receiver->HealthComp)
	//{
	//	HealthComponent->Heal(GetHealAmount());

	//	if (Request.AreSenderAndReceiverTheSame())
	//	{
	//		Result.Log.Messages.Add(
	//			FText::Format(
	//				LOCTEXT("TrpgCombatPerformAction", "{0} heals himself {1} HP"),
	//				FText::FromString(Request.Sender->GetWarriorName()),
	//				FText::FromString(FString::SanitizeFloat(GetHealAmount(), 0))
	//			)
	//		);
	//	}
	//	else
	//	{
	//		Result.Log.Messages.Add(
	//			FText::Format(
	//				LOCTEXT("TrpgCombatPerformAction", "{0} heals {1} {2} HP"),
	//				FText::FromString(Request.Sender->GetWarriorName()),
	//				FText::FromString(Request.Receiver->GetWarriorName()),
	//				FText::FromString(FString::SanitizeFloat(GetHealAmount(), 0))
	//			)
	//		);
	//	}
	//}
}

#undef LOCTEXT_NAMESPACE