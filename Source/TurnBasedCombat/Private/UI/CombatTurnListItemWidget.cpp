// Created by Bionic Ape. All Rights Reserved.


#include "UI/CombatTurnListItemWidget.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "UI/CombatUITurnInfo.h"

bool UCombatTurnListItemWidget::Initialize()
{
	if (Super::Initialize())
	{
		HiddeTurn();
		HiddeDead();
		return true;
	}
	return false;
}

void UCombatTurnListItemWidget::ShowTurn()
{
	PanelTurnIndicator->SetVisibility(ESlateVisibility::Visible);
}

void UCombatTurnListItemWidget::HiddeTurn()
{
	PanelTurnIndicator->SetVisibility(ESlateVisibility::Hidden);
}

void UCombatTurnListItemWidget::ShowDead()
{
	PanelDead->SetVisibility(ESlateVisibility::Visible);
}

void UCombatTurnListItemWidget::HiddeDead()
{
	PanelDead->SetVisibility(ESlateVisibility::Hidden);
}

void UCombatTurnListItemWidget::SetDead(bool IsDead)
{
	if (IsDead)
	{
		ShowDead();
	}
	else 
	{
		HiddeDead();
	}
}

void UCombatTurnListItemWidget::Configure()
{
	CharacterName->SetText(FText::FromString(Info->CombatantName));
}



void UCombatTurnListItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Info = Cast<UCombatUITurnInfo>(ListItemObject);
	if (Info == nullptr)
		return;

	Configure();
	//if (Info->Pos == 0)
	//	ShowTurn();
	//Configurar el nombre
}
