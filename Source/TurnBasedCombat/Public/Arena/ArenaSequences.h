// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ArenaSequences.generated.h"

class UActionType;
class ULevelSequence;

/**
 *
 */
UCLASS()
class TURNBASEDCOMBAT_API UArenaSequences : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<UActionType*, ULevelSequence*> Sequences;
};
