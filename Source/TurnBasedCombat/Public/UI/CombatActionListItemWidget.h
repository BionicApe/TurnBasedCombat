// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/ActionListWidget.h"
#include "UI/ActionListItemWidget.h"
#include "CombatActionListItemWidget.generated.h"

class UButton;
class UTextBlock;
class UActionType_Trpg;
struct FActionByActionsSetResult;
class UActionListInfo;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UCombatActionListItemWidget : public UActionListItemWidget
{
	GENERATED_BODY()
#pragma region Attributes
public:
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* APValue;

private:
#pragma endregion Attributes

#pragma region Methods
public:
	//UFUNCTION()
	//void Configure();
protected:
	void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private:
#pragma endregion Methods 
};
