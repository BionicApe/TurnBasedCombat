// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Fight.h"
#include "StrategistMenuWidget.generated.h"

class UTextBlock;
class UButton;
class APlayerCombatPawn;
class UImage;
class UInventoryItem;
class UPanelWidget;


/**
 *
 */
UCLASS()
class TURNBASEDCOMBAT_API UStrategistMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	APlayerCombatPawn* MyPawn;

	//UPROPERTY(meta = (BindWidget))
	//UButton* FinishTurnButton;

	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* CombatantName;
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ActionsPanel;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ActionPointsPanel;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* RemainingTimePanel;

	UPROPERTY(meta = (BindWidget))
	UImage* SelectedWeaponImage;

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
	virtual void NativeConstruct() override;
	
	APlayerCombatPawn* GetPlayerCombatPawn() const;	
	
	UFUNCTION()
	void OnFinishTurnButtonClicked();

protected:
	UFUNCTION()
	void UpdateItem(UInventoryItem * Item);

	UFUNCTION()
	void UpdateActionPoints(unsigned int NewActionPoints);

	UFUNCTION()
	void UpdateActions(unsigned int Actions);

	UFUNCTION()
	void UpdateRemainingSeconds(unsigned int Seconds);

	UFUNCTION()
	void MyTurn();
	UFUNCTION()
	void MyTurnEnds();
	UFUNCTION()
	void TurnUpdate(bool IsMyTurn);
	UFUNCTION()
	void UpdateCurrentTurnCombatant(FFightTurn CombatantInfo);
	UFUNCTION()
	void ConfigureFightListeners(AFight* Fight);
	UFUNCTION()
	void ConfigureCombatantListeners(ACombatant* Combatant);
};
