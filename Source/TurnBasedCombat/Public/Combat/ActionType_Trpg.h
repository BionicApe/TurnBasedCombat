// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActionType.h"
#include "TrpgCombatTypes.h"
#include "ActionType_Trpg.generated.h"

UENUM(BlueprintType)
enum class EActionTypeValue_Trpg : uint8
{
	ATTACK		= 0,
    FIRST_AID	= 1,
    ITEM		= 2, 
	SPECIAL		= 3
};

class ACombatant;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UActionType_Trpg : public UActionType
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "ActionType")
	unsigned int ActionPoints;

	UPROPERTY(EditDefaultsOnly, Category = "ActionType")
	unsigned int Actions = 1;

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

	UActionType_Trpg();


	UFUNCTION(BlueprintCallable, Category = "ActionType")
	int32 GetActionPoints() const { return ActionPoints; }

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	int32 GetActions() const { return Actions; }

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	virtual bool Validate(FTrpgPerformActionRequest const &Request, FTrpgPerformActionResult& Result) const;

	virtual bool PerformActionType(FFocusPerformAction Params) const override;

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	virtual void PerformAction(FTrpgPerformActionRequest const &Request, FTrpgPerformActionResult& Result) const {}

	virtual bool CanExecuteAction(AActor* ActionActor, AActor* ActionableActor) const override;
	virtual bool CanExecuteAction(ACombatant* ActionActor, ACombatant* ActionableActor) const;
};
