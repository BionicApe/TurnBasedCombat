// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActionType.h"
#include "FocusInteractionsTypes.h"
#include "ActionType_IncreaseAttributePoints.generated.h"

class AActor;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UActionType_IncreaseAttributePoints : public UActionType
{
	GENERATED_BODY()

public:

	virtual bool CanExecuteAction(AActor* ActionActor, AActor* ActionableActor) const override;

	virtual bool PerformActionType(FFocusPerformAction Params) const override;
};
