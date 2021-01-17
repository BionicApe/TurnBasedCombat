// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CombatantAnimInstance.generated.h"

class ACombatantSkeletal;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UCombatantAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AnimIndex = 0;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadWrite)
	ACombatantSkeletal* Combatant;

	
protected:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual bool TrySetCombatant();
};
