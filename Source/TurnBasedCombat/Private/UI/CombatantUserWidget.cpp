// Created by Bionic Ape. All Rights Reserved.

#include "UI/CombatantUserWidget.h"
#include "Components/TextBlock.h"
#include "Combatant.h"
#include "BAProfile.h"


bool UCombatantUserWidget::Initialize()
{
	if (Super::Initialize())
	{

	}
	return false;
}

void UCombatantUserWidget::SetCombatant(ACombatant* NewCombatant)
{
	Combatant = NewCombatant;

	if (CombatantName && Combatant && Combatant->Profile)
	{
		CombatantName->SetText(FText::FromString(Combatant->Profile->ProfileName));
	}
}