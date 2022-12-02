// Created by Bionic Ape. All Rights Reserved.


#include "Combat/ActionType_TrpgFirstAid.h"
#include "Containers/EnumAsByte.h"
#include "Components/HealthComponent.h"
#include "TrpgCombatTypes.h"
#include "Combatant.h"

#define LOCTEXT_NAMESPACE "ActionType_TrpgFirstAid"

UActionType_TrpgFirstAid::UActionType_TrpgFirstAid() : Super()
{
	ActionTypeValue = (uint8)EActionTypeValue_Trpg::FIRST_AID;
}

bool UActionType_TrpgFirstAid::Validate(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const
{

	if (!Super::Validate(Request, Result))
	{
		//Add Message to know what's wrong
		return false;
	}

	if (!Request.Receiver)
	{
		//Add Message to know what's wrong
		return false;
	}

	//if (!Request.Receiver->HealthComp)
	//{
	//	//Add Message to know what's wrong
	//	return false;
	//}
	//ACombatant* Combatant = Cast<ACombatant>(Request.Receiver);
	UHealthComponent* HealthComponent = Cast<UHealthComponent>(Request.Receiver->GetComponentByClass(UHealthComponent::StaticClass()));
	if (HealthComponent == nullptr)
	{
		return false;
	}
	if (HealthComponent->GetPercentage() == 1 || !HealthComponent->IsAlive())
		return false;
	return true;//CanExecuteAction(Request.Sender, Request.Receiver);
}

void UActionType_TrpgFirstAid::PerformAction(FTrpgPerformActionRequest const& Request, FTrpgPerformActionResult& Result) const
{
	if (UHealthComponent* HealthComponent = Cast<UHealthComponent>(Request.Receiver->GetComponentByClass(UHealthComponent::StaticClass())))
	{
		HealthComponent->Heal(GetHealAmount());
	}

	//	if (Request.AreSenderAndReceiverTheSame())
	//	{
	//		Result.Log.Messages.Add(
	//			FText::Format(
	//				LOCTEXT("TrpgCombatPerformAction", "{0} heals himself {1} HP"),
	//				FText::FromString(Request.Sender->GetWarriorName()),
	//				FText::FromString(FString::SanitizeFloat(GetHealAmount(), 0))
	//			)
	//		);
	//	}
	//	else
	//	{
	//		Result.Log.Messages.Add(
	//			FText::Format(
	//				LOCTEXT("TrpgCombatPerformAction", "{0} heals {1} {2} HP"),
	//				FText::FromString(Request.Sender->GetWarriorName()),
	//				FText::FromString(Request.Receiver->GetWarriorName()),
	//				FText::FromString(FString::SanitizeFloat(GetHealAmount(), 0))
	//			)
	//		);
	//	}
	//}
}

bool UActionType_TrpgFirstAid::CanExecuteAction(AActor* ActionActor, AActor* ActionableActor) const
{
	//bool bResult = true;//Super::CanExecuteAction(ActionActor, ActionableActor);

	UHealthComponent* HealthComponent = Cast<UHealthComponent>(ActionableActor->GetComponentByClass(UHealthComponent::StaticClass()));
	if (HealthComponent == nullptr)
	{
		return false;
	}
	if (HealthComponent->GetPercentage() == 1 || !HealthComponent->IsAlive())
		return false;

	return true;
}

#undef LOCTEXT_NAMESPACE