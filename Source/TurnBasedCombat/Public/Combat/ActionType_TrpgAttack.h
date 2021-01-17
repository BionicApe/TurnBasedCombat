// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Combat/ActionType_Trpg.h"
#include "TrpgCombatTypes.h"
#include "ActionType_TrpgAttack.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UActionType_TrpgAttack : public UActionType_Trpg
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "ActionType")
	float Damage;

public:

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	float GetDamage() const { return Damage; }

	UActionType_TrpgAttack();

	//virtual bool CanPerformAction(FtrpgPerformActionRequest& PerformActionRequest) const override;

	virtual bool Validate(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const override;
	
	virtual void PerformAction(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const override;
};
