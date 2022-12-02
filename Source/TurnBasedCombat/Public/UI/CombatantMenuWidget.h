// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/DynamicMenu.h"
#include "CombatantMenuWidget.generated.h"

class APlayerCombatPawn;
class ACombatant;
class UScaleBox;
class UBorder;
class UWidgetSwitcher;
class UButton;
class UInventoryListWidget;
class USizeBox;
class UCanvasPanel;
class UInventoryCategory;

/**
 *
 */
UCLASS()
class TURNBASEDCOMBAT_API UCombatantMenuWidget : public UDynamicMenu
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient)
	ACombatant* Combatant;

	//FVector2D MenuSize = FVector2D(300.f, 300.f);

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	USizeBox* WeaponsTab;

	UPROPERTY(meta = (BindWidget))
	UInventoryListWidget* InventoryList;

	UPROPERTY(meta = (BindWidget))
	UBorder* Combat_Border_Size;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* SelectWeaponsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* FinishTurnButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RepeatActionButton;

	UPROPERTY(meta = (BindWidget))
	UButton* Hexagon_Special;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Special;

	//UPROPERTY(meta = (BindWidgetAnim), Transient)
	//UWidgetAnimation* VisibilityAnimation;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ShowWeaponsAnim;

	//UPROPERTY(EditAnywhere, Config)
	//TSubclassOf<class UWidget> InventoryListClass;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float AnimPlaybackSpeed = 3.f;

	//bool bWantsToBeVisible = false;

public:


	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	APlayerCombatPawn* GetPlayerCombatPawn() const;
	//virtual void TogglePlayVisibilityAnim();
	UFUNCTION()
	void OnVisibilityAnimEnd_Implementation() override;
	UFUNCTION()
	void OnVisibilityAnimStart_Implementation() override;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowTrpgActionsWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void HideTrpgActionsWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void ActionClicked();

	UFUNCTION()
	void OnRepeatActionClicked();
	
	UFUNCTION()
	void OnSelectWeaponsButtonClicked();

	UFUNCTION()
	void OnFinishTurnButtonClicked();

	UFUNCTION()
	void OnSpecialButtonClicked();

	UFUNCTION(Blueprintcallable)
	void OnListItemByCategory(UInventoryCategory* Category);


};
