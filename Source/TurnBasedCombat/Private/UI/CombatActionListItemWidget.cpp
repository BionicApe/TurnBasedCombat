// Created by Bionic Ape. All Rights Reserved.


#include "UI/CombatActionListItemWidget.h"
#include "Interfaces/UseInventoryItem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Interfaces/InventoryOwner.h"
#include "Combat/ActionType_Trpg.h"
#include "UI/ActionListWidget.h"
#include "Components/FocusTracerComponent.h"
#include "UI/ActionListInfo.h"

void UCombatActionListItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);
	UActionType_Trpg* Action = Cast<UActionType_Trpg>(ActionInfo->Info.Actions[ActionInfo->Index].Action);
	if (Action)
	{
		ActionName->SetText(FText::FromName(Action->GetActionName()));
		//ActionName->Text = FText::FromName(ActionItem->GetActionName());
		APValue->SetText(FText::FromString(FString::FromInt(Action->GetActionPoints())));
	//	if (IUseInventoryItem* UseInventoryItem = Cast<IUseInventoryItem>(ActionButton))
	//	{
	//		UseInventoryItem->SetInventoryItem(InventoryItem);
	//	}
	}
}
