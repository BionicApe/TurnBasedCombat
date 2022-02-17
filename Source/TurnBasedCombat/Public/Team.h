// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Team.generated.h"

//class UFighterComponent;
class UFighterProfile;
class ITurnBasedStrategist;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UTeam : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = TurnBasedCombat, EditDefaultsOnly, BlueprintReadOnly, Replicated)
	TArray<UFighterProfile*> Profiles;

public:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
