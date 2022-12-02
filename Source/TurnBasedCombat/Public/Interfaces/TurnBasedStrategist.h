// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TurnBasedStrategist.generated.h"

class UBAUser;
class UBAProfile;
class AFight;
class ACombatant;
class UBAProfile;
class UActionType;

UINTERFACE(MinimalAPI)
class UTurnBasedStrategist : public UInterface
{
	GENERATED_BODY()
};

class TURNBASEDCOMBAT_API ITurnBasedStrategist
{
	GENERATED_BODY()

public:

	virtual void StartFight(AFight* Fight) = 0;
	virtual void StartTurn(UBAProfile* FighterProfile, ACombatant* NewCombatant) = 0;
	virtual void EndTurn() = 0;
	virtual void Configure(UBAProfile* FighterProfile, ACombatant* NewCombatant) = 0;
	virtual UBAProfile* GetCurrentProfile() = 0;
	virtual void NotifyFightFinish(AFight* FinishedFight) = 0;
	virtual AFight* GetFight() = 0;
	virtual bool ValidateAction(const UActionType* Action)=0;
	virtual ACombatant* GetCurrentCombatant()=0;
	//virtual void GetBAProfiles(TSet<UBAProfile*>& OutProfiles) const = 0;
	//virtual bool OwnsProfile(UBAProfile* Profile) const = 0;
};
