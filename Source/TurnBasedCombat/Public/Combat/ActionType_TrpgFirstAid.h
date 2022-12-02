// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Combat/ActionType_Trpg.h"
#include "TrpgCombatTypes.h"
#include "ActionType_TrpgFirstAid.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UActionType_TrpgFirstAid : public UActionType_Trpg
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "ActionType")
	float HealAmount = 35.f;

public:

	UActionType_TrpgFirstAid();
	
	UFUNCTION(BlueprintCallable, Category = "ActionType")
	float GetHealAmount() const { return HealAmount; }
	virtual bool Validate(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const override;
	virtual void PerformAction(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const override;
	virtual bool CanExecuteAction(AActor* ActionActor, AActor* ActionableActor) const override;
};
