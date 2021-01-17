// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FocusableFilter.h"
#include "FocusableFilter_IsEnemy.generated.h"

//class UTrpgWarriorComponent;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UFocusableFilter_IsEnemy : public UFocusableFilter
{
	GENERATED_BODY()

	//UPROPERTY(Transient)
	//UTrpgWarriorComponent* MyWarrior;

public:

	virtual bool IsAllowed(AActor* FocusableActor, UFocusableComponent* FocusableComponent) override;

};
