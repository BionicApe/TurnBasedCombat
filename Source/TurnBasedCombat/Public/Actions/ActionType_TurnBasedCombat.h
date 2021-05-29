// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActionType.h"

class AActor;
//#include "TrpgCombatTypes.h"
#include "ActionType_TurnBasedCombat.generated.h"

UENUM(BlueprintType)
enum class EActionTypeValue_TurnBasedCombat : uint8
{
	ATTACK		= 0,
    FIRST_AID	= 1,
    ITEM		= 2, 
	SPECIAL		= 3
};

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UActionType_TurnBasedCombat : public UActionType
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "ActionType")
	int32 ActionPoints;

	UPROPERTY(EditDefaultsOnly, Category = "ActionType")
	bool bExecuteOnEverything = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "ActionType")
	bool bExecuteOnSelf = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "ActionType")
	bool bExecuteOnEnemy = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "ActionType")
	bool bExecuteOnAlly = true;

	UPROPERTY(EditDefaultsOnly, Category = "ActionType")
	bool bRequiresSender = true;

	UPROPERTY(EditDefaultsOnly, Category = "ActionType")
	bool bRequiresReceiver = true;

public:

	UActionType_TurnBasedCombat();
	
	virtual bool CanExecuteAction(AActor* ActionActor, AActor* ActionableActor) const override;

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	int32 GetActionPoints() const { return ActionPoints; } 
	

	//UFUNCTION(BlueprintCallable, Category = "ActionType")
	//virtual bool Validate(FTrpgPerformActionRequest const &Request, FTrpgPerformActionResult& Result) const;

	//UFUNCTION(BlueprintCallable, Category = "ActionType")
	//virtual void PerformAction(FTrpgPerformActionRequest const &Request, FTrpgPerformActionResult& Result) const {}

};
