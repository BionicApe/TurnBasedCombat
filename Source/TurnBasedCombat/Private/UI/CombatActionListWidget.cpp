// Created by Bionic Ape. All Rights Reserved.


#include "UI/CombatActionListWidget.h"
#include "Components/ListView.h"
#include "Interfaces/InventoryOwner.h"
#include "PlayerCombatPawn.h"
#include "Inventory/InventoryItem.h"
#include "ActionsSet.h"
#include "Components/FocusableComponent.h"
#include "ActionType.h"
#include "UI/ActionListItemWidget.h"
#include "FocusInteractionsTypes.h"
#include "Combatant.h"
#include "UI/ActionListInfo.h"


void UCombatActionListWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCombatActionListWidget::WidgetLoaded(UUserWidget& NewWidget)
{
	Super::WidgetLoaded(NewWidget);
	UActionListItemWidget* Widget = Cast<UActionListItemWidget>(&NewWidget);
	if (Widget == nullptr)
		return;
	//IUseInventoryItem::Execute_SetInventoryItem(Widget->InventoryButton, Widget->InventoryItem);//This is implemented in BP
	if (APlayerCombatPawn* MyPawn = Cast<APlayerCombatPawn>(GetOwningPlayerPawn())) {
		if (ACombatant* Combatant = MyPawn->Combatant)
		{
			//Widget->ActionButton->SetIsEnabled(Combatant->CanUseThisItem(Widget->InventoryItem));
		}
	}

}
