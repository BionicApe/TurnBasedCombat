// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatantMenuWidget.generated.h"

class APlayerCombatPawn;
class ACombatant;
class UScaleBox;
class UBorder;
class UWidgetSwitcher;
class UButton;
class UInventoryListWidget;

/**
 *
 */
UCLASS()
class TURNBASEDCOMBAT_API UCombatantMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient)
	ACombatant* Combatant;

	//FVector2D MenuSize = FVector2D(300.f, 300.f);

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* WeaponsTab;

	UPROPERTY(meta = (BindWidget))
	UInventoryListWidget* InventoryList;

	UPROPERTY(meta = (BindWidget))
	UBorder* Combat_Border_Size;

	UPROPERTY(meta = (BindWidget))
	UButton* SelectWeaponsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* FinishTurnButton;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* VisibilityAnimation;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ShowWeaponsAnim;

	//UPROPERTY(EditAnywhere, Config)
	//TSubclassOf<class UWidget> InventoryListClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimPlaybackSpeed = 3.f;

	bool bWantsToBeVisible = false;

public:


	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	APlayerCombatPawn* GetPlayerCombatPawn() const;
	virtual void TogglePlayVisibilityAnim();
	UFUNCTION()
	void OnVisibilityAnimEnd();
	UFUNCTION()
	void OnVisibilityAnimStart();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowTrpgActionsWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void HideTrpgActionsWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void ActionClicked();
	
	UFUNCTION()
	void OnSelectWeaponsButtonClicked();

	UFUNCTION()
	void OnFinishTurnButtonClicked();

};
