// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BAProfile.h"
#include "FighterProfile.generated.h"

class UInventory;
class UTeam;

/**
 *
 */
UCLASS()
class TURNBASEDCOMBAT_API UFighterProfile : public UBAProfile
{
	GENERATED_BODY()

public:	

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "ActionType")
	UInventory* Inventory;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "ActionType")
	int32 ActionPoints = 100;

	UPROPERTY(BlueprintReadWrite, Category = TrpgCombat)
	int32 AmountOfActionsPerTurn = 2;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "ActionType")
	UTeam* Team;

public:

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
