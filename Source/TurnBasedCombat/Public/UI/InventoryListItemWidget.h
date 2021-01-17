// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "InventoryListItemWidget.generated.h"

class UButton;
class UInventoryItem;

//TODO: Move this class to InventorySystem --BIG TODO--

/**
 *	
 */
UCLASS()
class TURNBASEDCOMBAT_API UInventoryListItemWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UButton* InventoryButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryItem* InventoryItem;


public:

	virtual bool Initialize() override;

	bool IsListItemSelectable() const override { return InventoryItem != nullptr; }

	UFUNCTION()
	void OnButtonClicked();

protected:

	void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	//void NativeOnItemSelectionChanged(bool bIsSelected) override;
	//void NativeOnItemExpansionChanged(bool bIsExpanded) override;
	//void NativeOnEntryReleased() override;
};
