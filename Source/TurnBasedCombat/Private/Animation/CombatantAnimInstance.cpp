// Created by Bionic Ape. All Rights Reserved.


#include "Animation/CombatantAnimInstance.h"
#include "CombatantSkeletal.h"
#include "Inventory/InventoryItem.h"
#include "Combatant.h"
#include "Components/InventoryComponent.h"

void UCombatantAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Combatant || TrySetCombatant())
	{
		if (UInventoryItem* InvetoryItem = Combatant->InventoryComp->SelectedItem)
		{
			AnimIndex = InvetoryItem->FightingStyle;
		}
	}
}

bool UCombatantAnimInstance::TrySetCombatant()
{
	Combatant = Cast<ACombatantSkeletal>(GetOwningActor());
	return Combatant != nullptr;
}
