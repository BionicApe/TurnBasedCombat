// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TurnBasedCombatLib.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TURNBASEDCOMBAT_API UTurnBasedCombatLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "TurnBasedCombat", meta = (WorldContext = "WorldContextObject"))
	static AArena* FindArena(const UObject* WorldContextObject, FVector const& Location);
};
