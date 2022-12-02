// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryListWidget.generated.h"

class UTileView;
class UInventoryItem;
class UInventoryCategory;

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

	UPROPERTY()
	UInventoryCategory* PreviousCategory = nullptr;

public:

	virtual bool Initialize() override;


	bool RefreshList(UInventoryCategory* Category);

	bool RefreshList();

protected:
	UFUNCTION()
	/**
	* @brief Hidde the HUD
	* 
	*/
	void Close();

	/**
	 * @brief Called when the widget of the items are loaded.
	 * 
	 * @param NewWidget: The widget 
	 */
	void WidgetLoaded(UUserWidget& NewWidget);
};
