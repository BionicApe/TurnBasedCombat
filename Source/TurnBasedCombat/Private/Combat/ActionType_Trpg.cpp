// Created by Bionic Ape. All rights reseved.

#include "Combat/ActionType_Trpg.h"
#include "Internationalization/Text.h"
#include "GameFramework/Actor.h"
#include "Interfaces/TurnBasedStrategist.h"
#include "BAProfile.h"
#include "Combatant.h"
#include "FocusInteractionsTypes.h"
#include "Fight.h"
#include "TurnBasedCombatLog.h"

#define LOCTEXT_NAMESPACE "UActionType_TrpgAttack"

UActionType_Trpg::UActionType_Trpg() : Super()
{
	MaxDistance = 500000.f;//Large number so it's always possible to focus it
}

bool UActionType_Trpg::Validate(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const
{
	Result.bIsSuccessful = true;

	if (bRequiresSender && !Request.Sender)
	{
		Result.bIsSuccessful = false;
		Result.Log.Messages.Add(LOCTEXT("TrpgCombatPerformAction", "This action requires a Sender"));
	}
	if (bRequiresReceiver && !Request.Receiver)
	{
		Result.bIsSuccessful = false;
		Result.Log.Messages.Add(LOCTEXT("TrpgCombatPerformAction", "This action requires a Receiver"));
	}

	if (ACombatant* CombatantSender = Cast<ACombatant>(Request.Sender))
	{
		if (ActionPoints > 0 && CombatantSender->ActionPoints < ActionPoints)
		{
			Result.bIsSuccessful = false;
			Result.Log.Messages.Add(LOCTEXT("TrpgCombatPerformAction", "There's not enough action Points for this action"));
		}
		if (CombatantSender->RemainingActions <= 0)
		{
			Result.bIsSuccessful = false;
			Result.Log.Messages.Add(LOCTEXT("TrpgCombatPerformAction", "There's not enough remaining actions"));
		}
	}
	else
	{
		Result.bIsSuccessful = false;
		Result.Log.Messages.Add(LOCTEXT("TrpgCombatPerformAction", "This action requires a Combatant"));
	}

	return Result.bIsSuccessful;
}

bool UActionType_Trpg::PerformActionType(FFocusPerformAction Params) const
{
	/////////////////////////////////////////////////////////////////////////////
	// EXTRACTED FROM FIGHT.H
	/////////////////////////////////////////////////////////////////////////////
	//VALIDATIONS! TODO: MAYBE IT'S GOOD TO CHANGE THE FLOW

#pragma region Validations

	ITurnBasedStrategist* Strategist = Cast<ITurnBasedStrategist>(Params.ActionPawn);
	if (!Strategist)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("UActionType_Trpg::PerformActionType Strategist is null!"));
		return false;
	}

	ACombatant* Receiver = Cast<ACombatant>(Params.FocusedActor);
	if (!Receiver)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("UActionType_Trpg::PerformActionType FocusedCombatant is null!"));
		return false;
	}

	AFight* Fight = Receiver->Fight;
	if (!Fight)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("UActionType_Trpg::PerformActionType Fight is null!"));
		return false;
	}

	const FFightTurn* CurrentTurn = Fight->GetCurrentFightTurn();
	if (!CurrentTurn)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("UActionType_Trpg::PerformActionType CurrentTurn is null!"));
		return false;
	}

	if (CurrentTurn->Profile->Strategist != Strategist)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("UActionType_Trpg::PerformActionType CurrentTurn->Profile->Strategist != Strategist!"));
		return false;
	}

	ACombatant* Sender = CurrentTurn->Combatant;
	if (!Sender)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("UActionType_Trpg::PerformActionType Sender is null!"));
		return false;
	}


	return Fight->PerformAction(this, Strategist, Sender, Receiver);
}

bool UActionType_Trpg::CanExecuteAction(AActor* ActionActor, AActor* ActionableActor) const
{
	if (bExecuteOnEverything)
	{
		return true;
	}

	if (ITurnBasedStrategist* Strategist = Cast<ITurnBasedStrategist>(ActionActor))
	{
		if (UBAProfile* SenderProfile = Strategist->GetCurrentProfile())
		{
			if (ACombatant* ReceiverCombatant = Cast<ACombatant>(ActionableActor))
			{
				if (AFight const* const Fight = ReceiverCombatant->Fight)
				{
					bool const bIsEnemy = Fight->AreEnemies(SenderProfile, ReceiverCombatant->Profile);

					return (bIsEnemy && bExecuteOnEnemy) || (!bIsEnemy && bExecuteOnAlly) || (bExecuteOnSelf && (SenderProfile == ReceiverCombatant->Profile));
				}
			}
		}
	}
	return false;
}

#undef LOCTEXT_NAMESPACE