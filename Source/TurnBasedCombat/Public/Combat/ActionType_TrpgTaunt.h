// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Combat/ActionType_Trpg.h"
#include "ActionType_TrpgTaunt.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UActionType_TrpgTaunt : public UActionType_Trpg
{
	GENERATED_BODY()

public:
	
	virtual void PerformAction(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const override;

};
