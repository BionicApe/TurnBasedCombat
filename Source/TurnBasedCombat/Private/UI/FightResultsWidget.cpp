// Created by Bionic Ape. All Rights Reserved.


#include "UI/FightResultsWidget.h"
#include "PlayerCombatPawn.h"
#include "Fight.h"
#include "Combatant.h"
#include "BAProfile.h"
#include "Fight.h"
#include "TrpgCombatTypes.h"
#include "Components/TextBlock.h"


bool UFightResultsWidget::Initialize()
{
	bool bResult = Super::Initialize();
	if (bResult)
	{
		if (APlayerCombatPawn* MyPawn = GetPlayerCombatPawn())
		{
			if (MyPawn->Fight)
			{
				FTrpgFightResults const& FightResults = MyPawn->Fight->FightResults;

				if (FightResults.ResultType == EFightResultType::HAS_A_WINNER)
				{
					WinerNameTextBlock->SetText(FText::FromString(FightResults.WinnerTeamName));
				}
			}
		}
	}
	return bResult;
}

APlayerCombatPawn* UFightResultsWidget::GetPlayerCombatPawn() const
{
	return Cast<APlayerCombatPawn>(GetOwningPlayerPawn());
}