// Created by Bionic Ape. All Rights Reserved.


#include "UI/InventoryListWidget.h"
#include "Components/TileView.h"
#include "Inventory/InventoryItem.h"
#include "Interfaces/InventoryOwner.h"

bool UInventoryListWidget::Initialize()
{
	if (Super::Initialize())
	{
		return RefreshList();
	}
	return false;
}

bool UInventoryListWidget::RefreshList()
{
	if (ItemsTileView)
	{
		if (IInventoryOwner* InventoryOwner = Cast<IInventoryOwner>(GetOwningPlayerPawn()))
		{
			MyInventoryItems.Empty();
			InventoryOwner->GetInventoryItemsList(MyInventoryItems);
			ItemsTileView->SetListItems(MyInventoryItems);
			return true;
		}
	}
	return false;
}

