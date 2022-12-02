// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Fight.h"
#include "CombatTurnListWidget.generated.h"

class APlayerCombatPawn;
class AFight;
class ACombatant;
class UListView;
class UCombatTurnListItemWidget;
/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UCombatTurnListWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Attributes
public:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UListView* TurnListView;

	TMap<FString, ACombatant*> Combatants;
	TArray<UCombatTurnListItemWidget*> Widgets;
protected:
	int CurrentTurn = 0;
	int NCombatants=0;
	AFight* MyFight;
	ACombatant* MyCombatant;
	bool CreatingWindgets = false;
	bool UpdateTurnWileCreatingWidgets = false;

	int WidgetsToLoad = 0;
private:
#pragma endregion Attributes

#pragma region Methods
public:
	virtual bool Initialize() override;
	virtual void BeginDestroy() override;
protected:
	APlayerCombatPawn* GetPlayerCombatPawn() const;
	UFUNCTION()
	void ConfigureFightListeners(AFight* Fight);
	UFUNCTION()
	void ConfigureCombatantListeners(ACombatant* Combatant);
	UFUNCTION()
	void UpdateCurrentTurnCombatant(FFightTurn CombatantInfo);
	UFUNCTION()
	void NewRound(TArray<FFightTurn> Turns);


	/**
	 * @brief Called when the widget of the items are loaded.
	 *
	 * @param NewWidget: The widget
	 */
	virtual void WidgetLoaded(UUserWidget& NewWidget);

	/**
	 * @brief Foreach widget set if the combatant is alive or dead
	 * 
	 */
	void UpdateDeadOrAlive(UCombatTurnListItemWidget* Widget);
	//UFUNCTION()
	///**
	//* @brief Foreach widget set if the combatant is alive or dead
	//*
	//*/
	//void UpdateCombatantDeadOrAlive(ACombatant* Combatant, bool IsDead);

	UFUNCTION()
		/**
	 * @brief Foreach widget set if the combatant is alive or dead
	 *
	 * @param OldHealth:
	 * @param NewHealth:
	 * @param Owner:
	 */
	void OnCombatantHealthChange(int32 OldHealth, int32 NewHealth, AActor* HealthOwner);
private:
#pragma endregion Methods 
};
