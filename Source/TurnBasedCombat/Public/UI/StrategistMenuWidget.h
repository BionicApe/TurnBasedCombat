// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StrategistMenuWidget.generated.h"

class UTextBlock;
class UButton;
class APlayerCombatPawn;


/**
 *
 */
UCLASS()
class TURNBASEDCOMBAT_API UStrategistMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	//UPROPERTY(meta = (BindWidget))
	//UButton* FinishTurnButton;

	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* CombatantName;


	UPROPERTY(meta = (BindWidget))
	UTextBlock* ActionPoints;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentActions;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RemainingSeconds;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TurnCombatantName;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TurnInfo;

	UPROPERTY(EditDefaultsOnly, Category = Widgets)
	TSubclassOf<UUserWidget> WinnerWidgetClass;

public:

	virtual bool Initialize() override;
	
	virtual void HackUpdate();
	
	APlayerCombatPawn* GetPlayerCombatPawn() const;	
	
	UFUNCTION()
	void OnFinishTurnButtonClicked();

};
