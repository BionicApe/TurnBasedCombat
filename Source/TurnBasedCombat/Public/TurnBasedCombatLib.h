// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TurnBasedCombatLib.generated.h"

class AArena;
class UTrpgControlComponent;
class UFighterProfile;
class APlayerController;

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
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UTrpgControlComponent* GetTrpgControlComp(APlayerController* PlayerController);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UFighterProfile* GetMainFighterProfile(APlayerController* PlayerController);
};
