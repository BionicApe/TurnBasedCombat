// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryCategory.h"
#include "Interfaces/InventoryDAO.h"
#include "InventoryCategory_Consumable.generated.h"

class UInventory;
class UInventoryItem;
/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UInventoryCategory_Consumable : public UInventoryCategory
{
	GENERATED_BODY()
#pragma region Attributes
public:
protected:
private:
#pragma endregion Attributes

#pragma region Methods
public:
	virtual void Use_Implementation(UInventory* MyInventory, UInventoryItem* Item, UWorld* World) override;

	UFUNCTION()
	void OnInventoryResponse(FAsyncInventoryResponse Response);
protected:
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_RemoveItem(UInventory* MyInventory, UInventoryItem* Item, UWorld* World, int32 Ammount=1);
private:
#pragma endregion Methods 
	
};
