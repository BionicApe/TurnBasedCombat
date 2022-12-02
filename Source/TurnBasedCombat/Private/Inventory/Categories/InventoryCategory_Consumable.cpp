// Created by Bionic Ape. All Rights Reserved.


#include "Inventory/Categories/InventoryCategory_Consumable.h"
#include "Inventory/Inventory.h"
#include "Inventory/InventoryItem.h"
#include "Kismet/KismetTextLibrary.h"
#include "Interfaces/InventoryDAO.h"

#define LOCTEXT_NAMESPACE "UInventoryCategory_Consumable"

void UInventoryCategory_Consumable::Use_Implementation(UInventory* MyInventory, UInventoryItem* Item, UWorld* World)
{
	//UInventoryControlComponent* InventoryControl = UInventoryFunctionLibrary::GetInventoryControlComp(GetOwningPlayer());
	Server_RemoveItem(MyInventory, Item, World);
}


void UInventoryCategory_Consumable::Server_RemoveItem_Implementation(UInventory* MyInventory, UInventoryItem* Item, UWorld* World, int32 Ammount)
{
#pragma region ValidationCheck

	if (!MyInventory || !Item)
	{
		const FText ErrorText = LOCTEXT("InventoryNotCointained", "!Store || !BuyerInventory || !SellerInventory || !SellerEntry is/are null");
		UE_LOG(LogTemp, Error, TEXT("UInventoryControlComponent::Server_BuyItem_Implementation: %s"), *ErrorText.ToString());
		return;
	}

	if (Ammount < 1)
	{
		const FText ErrorText = FText::Format
		(
			LOCTEXT("NotBelongToStore", "The AmountOfItems to buy are less than 1: AmountOfItems:{0}"),
			UKismetTextLibrary::Conv_IntToText(Ammount)
		);
		UE_LOG(LogTemp, Error, TEXT("UInventoryControlComponent::Server_BuyItem_Implementation: %s"), *ErrorText.ToString());
	}

	if (!World || !World->GetGameInstance())
	{
		UE_LOG(LogTemp, Error, TEXT("The World and/or GameInstance are null"));
		return;
	}

	IInventoryDAOOwner* InventoryDaoOwner = Cast<IInventoryDAOOwner>(World->GetGameInstance());
	if (!InventoryDaoOwner)
	{
		UE_LOG(LogTemp, Error, TEXT("The GameInstance doesn't implement IInventoryDaoOwner"));
		return;
	}

	IInventoryDAO* Dao = InventoryDaoOwner->GetInventoryDAO();
	if (!Dao)
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryDaoOwner->GetInventoryDAO() is null"));
		return;
	}
#pragma endregion ValidationCheck


	//After all the validation, Sell the Item
	bool bIsSold = MyInventory->Remove(Item, Ammount, 0);
	//TODO react if we can't remove the item on ddbb.
	// This funciton is used in combat, we can't stop the fight with delay if there is a ddbb error. We should think with this restriction.

	if (bIsSold)
	{
		FInventoryResponseDelegate Delegate;
		Delegate.BindUObject(this, &UInventoryCategory_Consumable::OnInventoryResponse);
		Dao->Update(MyInventory, Delegate);
	}
	else
	{
		//Client_Notify(bIsSold, TEXT("The item couldn't be sold"));
	}
}

void UInventoryCategory_Consumable::OnInventoryResponse(FAsyncInventoryResponse Response)
{
}
#undef LOCTEXT_NAMESPACE // "InventoryControlComponent"
