// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CombatUITurnInfo.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UCombatUITurnInfo : public UObject
{
	GENERATED_BODY()
#pragma region Attributes
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString CombatantName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Pos;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ID;
protected:
private:
#pragma endregion Attributes

#pragma region Methods
public:
	static UCombatUITurnInfo* NEW();
	static UCombatUITurnInfo* NEW(FString _CombatantName, int _Pos, FString _ID);
protected:
private:
#pragma endregion Methods 
};
