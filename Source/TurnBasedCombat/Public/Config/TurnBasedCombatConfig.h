// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TurnBasedCombatConfig.generated.h"

class APawn;
class AAIController;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UTurnBasedCombatConfig : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UObject> AIStrategistClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly	)
	TSubclassOf<APawn> PlayerStrategistClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<APawn> ExplorationPawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AAIController> NpcAiControllerClass;

};
