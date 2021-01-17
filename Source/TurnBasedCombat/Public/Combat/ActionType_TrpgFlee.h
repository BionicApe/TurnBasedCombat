// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TrpgCombatTypes.h"
#include "ActionType_TrpgSpecial.h"
#include "ActionType_TrpgFlee.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UActionType_TrpgFlee : public UActionType_TrpgSpecial
{
	GENERATED_BODY()

public:

	int32 SuccessRate = 25;

	UActionType_TrpgFlee();

	virtual bool Validate(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const override;

	virtual void PerformAction(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const override;
};
