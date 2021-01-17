// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryListWidget.generated.h"

class UTileView;
class UInventoryItem;

/**
 *
 */
UCLASS()
class TURNBASEDCOMBAT_API UInventoryListWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTileView* ItemsTileView;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>  ItemsTileViewEntryClass;

	UPROPERTY()
	TArray<UInventoryItem*> MyInventoryItems;

public:

	virtual bool Initialize() override;

	bool RefreshList();
};
