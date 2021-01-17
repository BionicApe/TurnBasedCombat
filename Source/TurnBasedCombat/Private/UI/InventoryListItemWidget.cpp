// Created by Bionic Ape. All Rights Reserved.


#include "UI/InventoryListItemWidget.h"
#include "Interfaces/UseInventoryItem.h"
#include "Components/Button.h"
#include "Interfaces/InventoryOwner.h"

bool UInventoryListItemWidget::Initialize()
{
	if (Super::Initialize())
	{
		if (InventoryButton)
		{
			InventoryButton->OnClicked.AddDynamic(this, &UInventoryListItemWidget::OnButtonClicked);
			return true;
		}
	}
	return false;
}

void UInventoryListItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	InventoryItem = Cast<UInventoryItem>(ListItemObject);

	if (InventoryItem)
	{
		if (IUseInventoryItem* UseInventoryItem = Cast<IUseInventoryItem>(InventoryButton))
		{
			UseInventoryItem->SetInventoryItem(InventoryItem);
		}
	}
}

void UInventoryListItemWidget::OnButtonClicked()
{
	if (IInventoryOwner* InventoryOwner = Cast<IInventoryOwner>(GetOwningPlayerPawn()))
	{
		InventoryOwner->SetSelectedItem(InventoryItem);
	}
}
