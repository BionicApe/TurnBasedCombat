// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/TurnBasedStrategist.h"
#include "AIStrategist.generated.h"

class AFight;
class UBAProfile;
class ACombatant;
class UActionType;

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class TURNBASEDCOMBAT_API UAIStrategist : public UObject, public ITurnBasedStrategist
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	UActionType* DefaultAction;

	//We do nothing
	void StartFight(AFight* NewFight) override;

	void StartTurn(UBAProfile* NewProfile, ACombatant* NewCombatant) override;
	void Configure(UBAProfile* FighterProfile, ACombatant* NewCombatant) override {};
	bool ValidateAction(const UActionType* Action) override { return true; };
	virtual ACombatant* GetCurrentCombatant() override { return nullptr; }
	void EndTurn() override;

	UBAProfile* GetCurrentProfile() override { return nullptr; }

	void NotifyFightFinish(AFight* FinishedFight) override;

	AFight* GetFight() override { return nullptr; }
};
