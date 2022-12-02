// Created by Bionic Ape. All Rights Reserved.


#include "UI/InventoryListWidget.h"
#include "Components/TileView.h"
#include "Inventory/InventoryItem.h"
#include "Interfaces/InventoryOwner.h"
#include "UI/Elements/DisButton.h"
#include "Interfaces/UseInventoryItem.h"
#include "UI/InventoryListItemWidget.h"
#include "Inventory/InventoryCategory.h"
#include "PlayerCombatPawn.h"
#include "Combatant.h"
#include "PlayerCombatPawn.h"

bool UInventoryListWidget::Initialize()
{
	if (Super::Initialize())
	{
		//return RefreshList();
	}
	return false;
}

bool UInventoryListWidget::RefreshList(UInventoryCategory* Category)
{
	if (ItemsTileView || Category == nullptr/*?? esto esta bien?*/)
	{

		if (IInventoryOwner* InventoryOwner = Cast<IInventoryOwner>(GetOwningPlayerPawn()))
		{
			//if (Category == PreviousCategory)
			//{
			//	SetVisibility(ESlateVisibility::Visible);
			//	return true;
			//}
			PreviousCategory = Category;
			MyInventoryItems.Empty();
			TArray<UInventoryItem*> InventoryItems;
			InventoryOwner->GetInventoryItemsList(InventoryItems);
			//if (APlayerCombatPawn* PlayerPawn = Cast<APlayerCombatPawn>(GetOwningPlayerPawn()))
			//{
			//	UInventoryItem* DefaultItem = PlayerPawn->GetDefaultInventoryItem();
			//	if(DefaultItem)
			//		InventoryItems.Add(DefaultItem);//añadir el item por defecto siempre.
			//}

			for (UInventoryItem* Item : InventoryItems)
			{
				if (Item->Category == Category)
				{
					MyInventoryItems.Add(Item);
				}
			}

			//Wrong way to do it
			ItemsTileView->ClearListItems();
			//ItemsTileView->RegenerateAllEntries();
			ItemsTileView->SetListItems(MyInventoryItems);

			ItemsTileView->OnEntryWidgetGenerated().AddUObject(this, &UInventoryListWidget::WidgetLoaded);  // AddUFunction(this, "P");

			//ItemsTileView reuse the previous widgets, so if it exist we have to update manually.
			for (UObject* Item : ItemsTileView->GetListItems()) {
				UUserWidget* NewWidget = ItemsTileView->GetEntryWidgetFromItem(Item);
				UInventoryListItemWidget* Widget = Cast<UInventoryListItemWidget>(NewWidget);
				if (Widget)
				{
					WidgetLoaded(*NewWidget);
				}
			}

			//TODO this must be the right way to do it but it's not working
			//ItemsTileView reuse the previous widgets, so if it exist we have to update manually.
			//int WidgetsToLoad = MyInventoryItems.Num();
			//int i = 0;
			//for (UObject* Item : ItemsTileView->GetListItems())
			//{
			//	UUserWidget* NewWidget = ItemsTileView->GetEntryWidgetFromItem(Item);
			//	UInventoryListItemWidget* Widget = Cast<UInventoryListItemWidget>(NewWidget);
			//	if (Widget)
			//	{
			//		if (i < WidgetsToLoad)
			//		{
			//			Widget->Update(Item);
			//			WidgetLoaded(*NewWidget);
			//			i++;
			//		}
			//		else
			//		{
			//			ItemsTileView->RemoveItem(Item);
			//		}
			//	}
			//}
			//for (; i < WidgetsToLoad; i++)
			//{

			//	ItemsTileView->AddItem(MyInventoryItems[i]);
			//	//ActionsListInfo.Add(UCombatUITurnInfo::NEW(Turns[i].CombatantInfo.CombatantName, i,Turns[i].CombatantInfo.ID.ToString()));
			//}

			ItemsTileView->OnEntryWidgetGenerated().AddUObject(this, &UInventoryListWidget::WidgetLoaded);

			SetVisibility(ESlateVisibility::Visible);
			return true;
		}
	}
	return false;
}

bool UInventoryListWidget::RefreshList()
{
	return RefreshList(PreviousCategory);
}

void UInventoryListWidget::Close()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryListWidget::WidgetLoaded(UUserWidget& NewWidget)
{
	UInventoryListItemWidget* Widget = Cast<UInventoryListItemWidget>(&NewWidget);
	Cast<UDisButton>(Widget->InventoryButton)->OnClicked.AddUniqueDynamic(this, &UInventoryListWidget::Close);
	Widget->Update();
	if (Widget->InventoryButton->GetClass()->ImplementsInterface(UUseInventoryItem::StaticClass()))
	{
		IUseInventoryItem::Execute_SetInventoryItem(Widget->InventoryButton, Widget->InventoryItem);//This is implemented in BP
		if (APlayerCombatPawn* MyPawn = Cast<APlayerCombatPawn>(GetOwningPlayerPawn())) {
			if (ACombatant* Combatant = MyPawn->Combatant)
			{
				Widget->InventoryButton->SetIsEnabled(Combatant->CanUseThisItem(Widget->InventoryItem));
			}
		}
	}

	return;
}



