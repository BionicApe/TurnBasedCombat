// Created by Bionic Ape. All Rights Reserved.


#include "UI/InventoryListItemWidget.h"
#include "Interfaces/UseInventoryItem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Interfaces/InventoryOwner.h"

#include "Inventory/Inventory.h"
#include "InventoryItemEntry.h"

bool UInventoryListItemWidget::Initialize()
{
	if (Super::Initialize())
	{
		if (InventoryButton)
		{
			InventoryButton->OnClicked.AddUniqueDynamic(this, &UInventoryListItemWidget::OnButtonClicked);
			return true;
		}
	}
	return false;
}

void UInventoryListItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Update(ListItemObject);
}

void UInventoryListItemWidget::OnButtonClicked()
{
	if (IInventoryOwner* InventoryOwner = Cast<IInventoryOwner>(GetOwningPlayerPawn()))
	{
		InventoryOwner->SetSelectedItem(InventoryItem);
	}
}

void UInventoryListItemWidget::Update()
{
	if (InventoryItem)
	{
		if (IUseInventoryItem* UseInventoryItem = Cast<IUseInventoryItem>(InventoryButton))
		{
			UseInventoryItem->SetInventoryItem(InventoryItem);
		}
		if (IInventoryOwner* InventoryOwner = Cast<IInventoryOwner>(GetOwningPlayerPawn()))
		{
			TArray<UInventoryItemEntry*> Entries = InventoryOwner->GetInventory()->GetEntries();
			for (auto Entry : Entries)
			{
				if (Entry->Item == InventoryItem)
				{
					AmountTextBlock->SetText(FText::FromString(FString::FromInt(Entry->GetAmount())));
					break;
				}
			}
		}
	}
}

void UInventoryListItemWidget::Update(UObject* ListItemObject)
{
	InventoryItem = Cast<UInventoryItem>(ListItemObject);
	Update();
}
