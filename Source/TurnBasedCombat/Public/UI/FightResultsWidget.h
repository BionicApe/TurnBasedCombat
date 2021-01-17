// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FightResultsWidget.generated.h"

class UTextBlock;
class UButton;
class APlayerCombatPawn;


/**
 *
 */
UCLASS()
class TURNBASEDCOMBAT_API UFightResultsWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WinerNameTextBlock;

public:

	virtual bool Initialize() override;

	APlayerCombatPawn* GetPlayerCombatPawn() const;	
	
};
