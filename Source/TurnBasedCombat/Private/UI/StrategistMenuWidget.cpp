// Created by Bionic Ape. All Rights Reserved.


#include "UI/StrategistMenuWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ScaleBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"
#include "FighterProfile.h"

#include "Animation/WidgetAnimation.h"

#include "UI/InventoryListWidget.h"

#include "PlayerCombatPawn.h"
#include "Fight.h"
#include "Combatant.h"
#include "BAProfile.h"
#include "Inventory/InventoryItem.h"


bool UStrategistMenuWidget::Initialize()
{
	bool bResult = Super::Initialize();
	if (bResult)
	{
		//FinishTurnButton->OnClicked.AddDynamic(this, &UStrategistMenuWidget::OnFinishTurnButtonClicked);
		MyPawn = GetPlayerCombatPawn();
		if (MyPawn)
		{
			MyPawn->OnItemChange.AddUniqueDynamic(this, &UStrategistMenuWidget::UpdateItem);
			UpdateItem(MyPawn->GetCurrentInventoryItem());
			if (MyPawn->Combatant)
			{
				ConfigureCombatantListeners(MyPawn->Combatant);
			}
			else 
			{
				MyPawn->OnCombatantReady.AddUniqueDynamic(this, &UStrategistMenuWidget::ConfigureCombatantListeners);
			}
		}
	}

	return bResult;
}

#pragma region ToDelete
void UStrategistMenuWidget::HackUpdate()
{
	/*if (MyPawn = GetPlayerCombatPawn())
	{
		if (AFight* Fight = MyPawn->Fight)
		{
			{
				if (FFightTurn const* FightTurn = Fight->GetCurrentFightTurn())
				{
					if (UBAProfile* Profile = FightTurn->Profile)
					{
						if (TurnCombatantName)
						{
							TurnCombatantName->SetText(FText::FromString(Profile->ProfileName));
						}
					}

					if (ACombatant* Combatant = FightTurn->Combatant)
					{
						if (ActionPoints)
						{
							ActionPoints->SetText(FText::FromString(FString::FromInt(Combatant->ActionPoints)));
						}
						if (CurrentActions)
						{
							CurrentActions->SetText(FText::FromString(FString::FromInt(Combatant->RemainingActions)));
						}
						if (RemainingSeconds)
						{
							RemainingSeconds->SetText(FText::FromString(FString::FromInt(Combatant->RemainingSeconds)));
						}

						if (TurnInfo)
						{
							if (Combatant == MyPawn->Combatant)
							{
								TurnInfo->SetText(FText::FromString(TEXT("It's your turn")));
							}
							else
							{
								TurnInfo->SetText(FText::FromString(TEXT("Wait for your turn")));
							}
						}
					}
				}
			}
		}
	}*/
}
void UStrategistMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
#pragma endregion
APlayerCombatPawn* UStrategistMenuWidget::GetPlayerCombatPawn() const
{
	return Cast<APlayerCombatPawn>(GetOwningPlayerPawn());
}

void UStrategistMenuWidget::OnFinishTurnButtonClicked()
{
	if (APlayerCombatPawn* PlayerCombatPawn = GetPlayerCombatPawn())
	{
		PlayerCombatPawn->EndTurn();
	}
}

void UStrategistMenuWidget::UpdateItem(UInventoryItem* Item)
{
	if (!SelectedWeaponImage || Item == nullptr)
	{
		return;
	}
	SelectedWeaponImage->SetBrushFromSoftTexture(Item->Icon);
}

void UStrategistMenuWidget::UpdateActionPoints(unsigned int NewActionPoints)
{
	if (ActionPoints)
	{
		ActionPoints->SetText(FText::FromString(FString::FromInt(NewActionPoints)));
	}
}

void UStrategistMenuWidget::UpdateActions(unsigned int Actions)
{
	if (CurrentActions)
	{
		CurrentActions->SetText(FText::FromString(FString::FromInt(Actions)));
	}
}

void UStrategistMenuWidget::UpdateRemainingSeconds(unsigned int Seconds)
{
	if(RemainingSeconds)
		RemainingSeconds->SetText(FText::FromString(FString::FromInt(Seconds)));
}

void UStrategistMenuWidget::MyTurn()
{
	if(TurnInfo)
		TurnInfo->SetVisibility(ESlateVisibility::Visible);
	if (ActionsPanel)
	{
		ActionsPanel->SetVisibility(ESlateVisibility::Visible);
	}
	if (ActionPointsPanel)
	{
		ActionPointsPanel->SetVisibility(ESlateVisibility::Visible);
	}
	if (RemainingTimePanel)
		RemainingTimePanel->SetVisibility(ESlateVisibility::Visible);
	if (SelectedWeaponImage)
		SelectedWeaponImage->SetVisibility(ESlateVisibility::Visible);
}

void UStrategistMenuWidget::MyTurnEnds()
{
	if (TurnInfo)
		TurnInfo->SetVisibility(ESlateVisibility::Hidden);
	if (ActionsPanel)
	{
		ActionsPanel->SetVisibility(ESlateVisibility::Hidden);
	}
	if (ActionPointsPanel)
	{
		ActionPointsPanel->SetVisibility(ESlateVisibility::Hidden);
	}
	if(RemainingTimePanel)
		RemainingTimePanel->SetVisibility(ESlateVisibility::Hidden);
	if(SelectedWeaponImage)
		SelectedWeaponImage->SetVisibility(ESlateVisibility::Hidden);
}

void UStrategistMenuWidget::TurnUpdate(bool IsMyTurn)
{
	if (IsMyTurn)
	{
		MyTurn();
	}
	else 
	{
		MyTurnEnds();
	}
}

void UStrategistMenuWidget::UpdateCurrentTurnCombatant(FFightTurn CombatantInfo)
{
	if(TurnCombatantName)
		TurnCombatantName->SetText(FText::FromString(CombatantInfo.CombatantInfo.CombatantName));
}

void UStrategistMenuWidget::ConfigureFightListeners(AFight* Fight)
{
	Fight->OnNewTurnCombatant.AddUniqueDynamic(this, &UStrategistMenuWidget::UpdateCurrentTurnCombatant);
	FFightTurn const* Turn = Fight->GetCurrentFightTurn();
	if(Turn)
		UpdateCurrentTurnCombatant(*Turn);
}

void UStrategistMenuWidget::ConfigureCombatantListeners(ACombatant* Combatant)
{
	if (Combatant == nullptr)
		return;

	Combatant->OnActionPointsChange.AddUniqueDynamic(this, &UStrategistMenuWidget::UpdateActionPoints);
	Combatant->OnRemainActionsChange.AddUniqueDynamic(this, &UStrategistMenuWidget::UpdateActions);
	Combatant->OnTurnUpdate.AddUniqueDynamic(this, &UStrategistMenuWidget::TurnUpdate);
	Combatant->OnRemainingSecondsChange.AddUniqueDynamic(this, &UStrategistMenuWidget::UpdateRemainingSeconds);
	TurnUpdate(Combatant->IsMyTurn());
	if (Combatant->Fight)
	{
		ConfigureFightListeners(Combatant->Fight);
	}
	else
	{
		Combatant->OnFightSetUp.AddUniqueDynamic(this, &UStrategistMenuWidget::ConfigureFightListeners);
	}
	UpdateActionPoints(Combatant->ActionPoints);
	UpdateActions(Combatant->RemainingActions);
}
