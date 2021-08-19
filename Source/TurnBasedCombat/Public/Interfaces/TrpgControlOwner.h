// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "TrpgControlOwner.generated.h"

class UTrpgControlComponent;

/**
*
*/
UINTERFACE(Blueprintable)
class TURNBASEDCOMBAT_API UTrpgControlOwner : public UInterface
{
	GENERATED_BODY()
};

class ITrpgControlOwner
{
	GENERATED_BODY()

public:

	virtual UTrpgControlComponent* GetTrpgControlComp() const = 0;
};