// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TurnBasedCombatConfig.generated.h"

class AAIController;
class APawn;

/**
 *
 */
UCLASS()
class TURNBASEDCOMBAT_API UTurnBasedCombatConfig : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, VisibleAnywhere)
	TSubclassOf<UObject> AIStrategistClass;

	UPROPERTY(EditDefaultsOnly, VisibleAnywhere)
	TSubclassOf<APawn> PlayerCombatPawnClass;

	UPROPERTY(EditDefaultsOnly, VisibleAnywhere)
	TSubclassOf<APawn> ExplorationPawnClass;

	UPROPERTY(EditDefaultsOnly, VisibleAnywhere)
	TSubclassOf<AAIController> NpcAiControllerClass;

public:


	UPROPERTY(EditDefaultsOnly, VisibleAnywhere)
	TSubclassOf<UObject> GetAIStrategistClass() const { return AIStrategistClass; }

	UPROPERTY(EditDefaultsOnly, VisibleAnywhere)
	TSubclassOf<APawn> GetPlayerCombatPawnClass() const { return PlayerCombatPawnClass; }

	UPROPERTY(EditDefaultsOnly, VisibleAnywhere)
	TSubclassOf<APawn> GetExplorationPawnClass() const { return ExplorationPawnClass; }

	UPROPERTY(EditDefaultsOnly, VisibleAnywhere)
	TSubclassOf<AAIController> GetNpcAiControllerClass() const { return NpcAiControllerClass; }

};
