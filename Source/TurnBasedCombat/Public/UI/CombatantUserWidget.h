// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatantUserWidget.generated.h"

class ACombatant;
class UTextBlock;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UCombatantUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient)
	ACombatant* Combatant;


	UPROPERTY(meta = (BindWidget))
	UTextBlock* CombatantName;

public:

	virtual bool Initialize() override;

	virtual void SetCombatant(ACombatant* Combatant);
};
