// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Combat/ActionType_TrpgSpecial.h"
#include "ActionType_TrpgSaveActionPoints.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UActionType_TrpgSaveActionPoints : public UActionType_TrpgSpecial
{
	GENERATED_BODY()

public:

	UActionType_TrpgSaveActionPoints();

	virtual void PerformAction(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const override;
};
