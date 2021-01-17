// Created by Bionic Ape. All Rights Reserved.


#include "Combat/ActionType_TrpgAttack.h"
#include "Containers/EnumAsByte.h"
#include "Components/HealthComponent.h"
#include "GameFramework/Pawn.h"
#include "TrpgCombatTypes.h"
#include "GameFramework/Actor.h"

#define LOCTEXT_NAMESPACE "UActionType_TrpgAttack"

UActionType_TrpgAttack::UActionType_TrpgAttack() : Super()
{
	ActionTypeValue = (uint8)EActionTypeValue_Trpg::ATTACK;

	bExecuteOnEverything = false;
	bExecuteOnSelf = false;
	bExecuteOnEnemy = true;
	bExecuteOnAlly = false;
}

bool  UActionType_TrpgAttack::Validate(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const
{
	if (!Super::Validate(Request, Result))
	{
		//Add Message to know what's wrong
		return false;
	}

	if (!Request.Receiver)
	{
		//Add Message to know what's wrong
		return false;
	}

	//if (!Request.Receiver->HealthComp)
	//{
	//	//Add Message to know what's wrong
	//	return false;
	//}
	return true;
}


void UActionType_TrpgAttack::PerformAction(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const
{
	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor * DamageCauser);

	if (Validate(Request, Result))
	{
		FDamageEvent Event;

		Request.Receiver->TakeDamage(Damage, Event, nullptr, Request.Sender);

		//Result.Log.Messages.Add(
		//	FText::Format(
		//		LOCTEXT("TrpgCombatPerformAction", "{0} attacks {1}  with a strenght of {2} leaving a health of {3}"),
		//		FText::FromString(Request.Sender->GetWarriorName()),
		//		FText::FromString(Request.Receiver->GetWarriorName()),
		//		FText::FromString(FString::SanitizeFloat(GetDamage(), 0)),
		//		FText::FromString(FString::SanitizeFloat(Request.Receiver->HealthComp->Health, 0))
		//	));

	}
}

#undef LOCTEXT_NAMESPACE