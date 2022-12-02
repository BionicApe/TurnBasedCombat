// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StrategistHUD.generated.h"

class ACombatant;
struct FFocusTraceInfo;
struct FTrpgFightResults;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStrategistHUD : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class TURNBASEDCOMBAT_API IStrategistHUD
{
	GENERATED_BODY()

public:

	virtual void ShowStartCombat() = 0;

	virtual void ShowStartTurn() = 0;

	virtual void ShowMenu(ACombatant* Combatant) = 0;

	virtual void ShowActionsMenu(FFocusTraceInfo Info) = 0;
	
	virtual void ShowFightResults(FTrpgFightResults FightResults) = 0;
};
