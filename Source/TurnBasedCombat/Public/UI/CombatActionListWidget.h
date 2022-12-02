// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "DynamicMenu.h"
#include "UI/ActionListWidget.h"
//#include "FocusInteractionsTypes.h"
#include "CombatActionListWidget.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UCombatActionListWidget : public UActionListWidget
{
	GENERATED_BODY()
#pragma region Attributes
public:

protected:
private:
#pragma endregion Attributes

#pragma region Methods
public:
	virtual void NativeConstruct() override;
protected:

	/**
	 * @brief Called when the widget of the items are loaded.
	 *
	 * @param NewWidget: The widget
	 */
	virtual void WidgetLoaded(UUserWidget& NewWidget) override;
private:
#pragma endregion Methods 
};
