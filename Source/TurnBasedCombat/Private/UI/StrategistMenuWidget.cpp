// Created by Bionic Ape. All Rights Reserved.


#include "UI/StrategistMenuWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/ScaleBox.h"
#include "Components/WidgetSwitcher.h"

#include "Animation/WidgetAnimation.h"

#include "UI/InventoryListWidget.h"

#include "PlayerCombatPawn.h"
#include "Fight.h"
#include "Combatant.h"
#include "BAProfile.h"


bool UStrategistMenuWidget::Initialize()
{
	bool bResult = Super::Initialize();
	if (bResult)
	{
		//FinishTurnButton->OnClicked.AddDynamic(this, &UStrategistMenuWidget::OnFinishTurnButtonClicked);
	}

	return bResult;
}

#pragma region ToDelete
void UStrategistMenuWidget::HackUpdate()
{
	if (APlayerCombatPawn* MyPawn = GetPlayerCombatPawn())
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
	}
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
