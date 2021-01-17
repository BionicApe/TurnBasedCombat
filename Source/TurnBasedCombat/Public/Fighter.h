// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Fighter.generated.h"

/**
 *
 */
UCLASS()
class TURNBASEDCOMBAT_API UFighter : public UObject
{
	GENERATED_BODY()

public:	

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

};
