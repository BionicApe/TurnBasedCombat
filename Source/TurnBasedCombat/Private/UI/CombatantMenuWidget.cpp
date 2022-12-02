// Created by Bionic Ape. All Rights Reserved.


#include "UI/CombatantMenuWidget.h"
#include "PlayerCombatPawn.h"
#include "Components/ScaleBox.h"
#include "Components/Border.h"
#include "Animation/WidgetAnimation.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "UI/InventoryListWidget.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanel.h"
#include "Inventory/InventoryCategory.h"

bool UCombatantMenuWidget::Initialize()
{
	bool bResult = Super::Initialize();
	if (bResult)
	{
		//if (VisibilityAnimation)
		//{
		//	FWidgetAnimationDynamicEvent OnAnimStart;
		//	OnAnimStart.BindDynamic(this, &UCombatantMenuWidget::OnVisibilityAnimStart);
		//	BindToAnimationStarted(VisibilityAnimation, OnAnimStart);
		//}

		//if (VisibilityAnimation)
		//{
		//	FWidgetAnimationDynamicEvent OnAnimEnd;
		//	OnAnimEnd.BindDynamic(this, &UCombatantMenuWidget::OnVisibilityAnimEnd);
		//	BindToAnimationFinished(VisibilityAnimation, OnAnimEnd);
		//}

		/*if (SelectWeaponsButton)
		{
			SelectWeaponsButton->OnClicked.AddDynamic(this, &UCombatantMenuWidget::OnSelectWeaponsButtonClicked);
		}*/

		if (FinishTurnButton)
		{
			FinishTurnButton->OnClicked.AddDynamic(this, &UCombatantMenuWidget::OnFinishTurnButtonClicked);
		}
		if (Hexagon_Special)
		{
			Hexagon_Special->OnClicked.AddDynamic(this, &UCombatantMenuWidget::OnSpecialButtonClicked);
		}
		if (RepeatActionButton)
		{
			RepeatActionButton->OnClicked.AddDynamic(this, &UCombatantMenuWidget::OnRepeatActionClicked);
		}
	}

	return bResult;
}

void UCombatantMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Combat_Border_Size->SetVisibility(ESlateVisibility::Hidden);
}

APlayerCombatPawn* UCombatantMenuWidget::GetPlayerCombatPawn() const
{
	return Cast<APlayerCombatPawn>(GetOwningPlayerPawn());
}

//void UCombatantMenuWidget::TogglePlayVisibilityAnim()
//{
//	bWantsToBeVisible = !bWantsToBeVisible;
//	float const AnimationCurrentTime = GetAnimationCurrentTime(VisibilityAnimation);
//	PlayAnimation(VisibilityAnimation, AnimationCurrentTime, 1, bWantsToBeVisible ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse, AnimPlaybackSpeed);
//}

void UCombatantMenuWidget::OnVisibilityAnimEnd_Implementation()
{
	if (!bWantsToBeVisible)
	{
		Combat_Border_Size->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCombatantMenuWidget::OnVisibilityAnimStart_Implementation()
{
	if (bWantsToBeVisible)
	{
		WidgetSwitcher->SetActiveWidgetIndex(0);
		Combat_Border_Size->SetVisibility(ESlateVisibility::Visible);
		if (InventoryList)
			InventoryList->RefreshList();
		if (RepeatActionButton)
		{
			if (APlayerCombatPawn* PlayerCombatPawn = GetPlayerCombatPawn())
			{
				bool p = PlayerCombatPawn->CanPerformLastAction();
				RepeatActionButton->SetIsEnabled(p);
			}
		}
	}
}

void UCombatantMenuWidget::OnRepeatActionClicked()
{
	if (RepeatActionButton)
	{
		if (APlayerCombatPawn* PlayerCombatPawn = GetPlayerCombatPawn())
		{
			Combat_Border_Size->SetVisibility(ESlateVisibility::Hidden);
			PlayerCombatPawn->Server_PerformLastAction();
		}
	}
}

void UCombatantMenuWidget::OnSelectWeaponsButtonClicked()
{
	//InventoryList->RefreshList();
	WidgetSwitcher->SetActiveWidget(WeaponsTab);
	//WidgetSwitcher->SetActiveWidgetIndex(2);
	//PlayAnimation(ShowWeaponsAnim, 0.f, 1, EUMGSequencePlayMode::Forward, AnimPlaybackSpeed);
}

void UCombatantMenuWidget::OnFinishTurnButtonClicked()
{
	if (APlayerCombatPawn* PlayerCombatPawn = GetPlayerCombatPawn())
	{
		PlayerCombatPawn->RequestFinishTurn();
	}
}

void UCombatantMenuWidget::OnSpecialButtonClicked()
{
	WidgetSwitcher->SetActiveWidget(Special);
}

void UCombatantMenuWidget::OnListItemByCategory(UInventoryCategory* Category)
{
	InventoryList->RefreshList(Category);
	WidgetSwitcher->SetActiveWidget(WeaponsTab);
}
