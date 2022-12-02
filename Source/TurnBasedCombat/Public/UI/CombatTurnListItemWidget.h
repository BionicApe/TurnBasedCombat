// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CombatTurnListItemWidget.generated.h"

class UPanelWidget;
class UTextBlock;
class UCombatUITurnInfo;
/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UCombatTurnListItemWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
#pragma region Attributes
public:
	UPROPERTY()
	UCombatUITurnInfo* Info;
protected:
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* PanelName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterName;
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* PanelDead;
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* PanelTurnIndicator;
private:
#pragma endregion Attributes

#pragma region Methods
public:
	virtual bool Initialize() override;
	/**
	 * @brief Show the turn indicator
	 * 
	 */
	void ShowTurn();
	/**
	 * @brief Hidde the turn indicator
	 *
	 */
	void HiddeTurn();

	void ShowDead();
	void HiddeDead();
	void SetDead(bool IsDead);
	void Configure();
protected:
	void NativeOnListItemObjectSet(UObject* ListItemObject) override;
private:
#pragma endregion Methods 
};
