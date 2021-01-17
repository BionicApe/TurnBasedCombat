// Created by Bionic Ape. All rights reseved.

#include "Actions/ActionType_TurnBasedCombat.h"
#include "Internationalization/Text.h"
#include "GameFramework/Actor.h"
#include "Interfaces/TurnBasedStrategist.h"
#include "BAProfile.h"
#include "Combatant.h"
#include "Fight.h"
#include "FighterProfile.h"

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

#undef LOCTEXT_NAMESPACE