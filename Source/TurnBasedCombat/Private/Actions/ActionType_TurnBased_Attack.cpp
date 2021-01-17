// Created by Bionic Ape. All Rights Reserved.


#include "Actions/ActionType_TurnBased_Attack.h"
#include "Actions/ActionType_TurnBasedCombat.h"
#include "Containers/EnumAsByte.h"
#include "Components/HealthComponent.h"
#include "GameFramework/Pawn.h"
#include "TrpgCombatTypes.h"
#include "GameFramework/Actor.h"

#define LOCTEXT_NAMESPACE "UActionType_TurnBased_Attack"

UActionType_TurnBased_Attack::UActionType_TurnBased_Attack() : Super()
{
	ActionTypeValue = (uint8)EActionTypeValue_TurnBasedCombat::ATTACK;

	bExecuteOnEverything = false;
	bExecuteOnSelf = false;
	bExecuteOnEnemy = true;
	bExecuteOnAlly = false;
}

//bool  UActionType_TurnBased_Attack::Validate(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const
//{
//	if (!Super::Validate(Request, Result))
//	{
//		//Add Message to know what's wrong
//		return false;
//	}
//
//	if (!Request.Receiver)
//	{
//		//Add Message to know what's wrong
//		return false;
//	}
//
//	if (!Request.Receiver->HealthComp)
//	{
//		//Add Message to know what's wrong
//		return false;
//	}
//	return true;
//}


void UActionType_TurnBased_Attack::PerformAction(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const
{
	//if (Validate(Request, Result))
	//{
	//	FDamageEvent Event;
	//	//Request.Receiver->HealthComp->TakeDamageNoInstigator(GetDamage(), Event, Request.Sender->WarriorPawn);//TODO: Maybe change this to use just the actor not the HealthComponent
	//	Request.Receiver->TakeDamage(GetDamage(), Event, Request.Sender->GetInstigator(), Request.Sender);

	//	Result.Log.Messages.Add(
	//		FText::Format(
	//			LOCTEXT("TrpgCombatPerformAction", "{0} attacks {1}  with a strenght of {2} leaving a health"),
	//			//FText::FromString(Request.Sender->GetWarriorName()),
	//			//FText::FromString(Request.Receiver->GetWarriorName()),
	//			FText::FromString(Request.Sender->GetName()),
	//			FText::FromString(Request.Receiver->GetName()),
	//			FText::FromString(FString::SanitizeFloat(GetDamage(), 0))
	//		)
	//	);
	//}
}

#undef LOCTEXT_NAMESPACE