// Created by Bionic Ape. All rights reseved.

#include "Actions/ActionType_TurnBasedCombat.h"
#include "Internationalization/Text.h"
#include "GameFramework/Actor.h"
#include "Interfaces/TurnBasedStrategist.h"
#include "BAProfile.h"
#include "Combatant.h"
#include "Fight.h"
#include "FighterProfile.h"
#include "TurnBasedCombatLog.h"


#define LOCTEXT_NAMESPACE "UActionType_TurnBasedCombatAttack"

UActionType_TurnBasedCombat::UActionType_TurnBasedCombat() : Super()
{
	MaxDistance = 500000.f;//Large number so it's always possible to focus it
}

bool UActionType_TurnBasedCombat::CanExecuteAction(AActor* ActionActor, AActor* ActionableActor) const
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

bool UActionType_TurnBasedCombat::PerformActionType(FFocusPerformAction Params) const
{

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
	if (Sender)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("UActionType_Trpg::PerformActionType Sender is null!"));
		return false;
	}


	return Fight->PerformAction(this, Strategist, Sender, Receiver);
}

#undef LOCTEXT_NAMESPACE