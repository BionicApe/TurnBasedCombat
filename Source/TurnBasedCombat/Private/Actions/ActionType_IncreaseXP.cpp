// Created by Bionic Ape. All Rights Reserved.


#include "Actions/ActionType_IncreaseXP.h"
#include "FocusInteractionsTypes.h"
#include "Components/TrpgControlComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Mockup/MockupFocusable.h"
#include "TurnBasedCombatLib.h"

#define LOCTEXT_NAMESPACE "UActionType_IncreaseXP"

bool UActionType_IncreaseXP::CanExecuteAction(AActor* ActionActor, AActor* ActionableActor) const
{
	return true;
}

bool UActionType_IncreaseXP::PerformActionType(FFocusPerformAction Params) const
{

#pragma region ErrorChecking
	UE_LOG(LogTemp, Warning, TEXT("UActionType_IncreaseXP::PerformActionType()"));


	if (!Params.ActionController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UActionType_IncreaseXP::PerformActionType() No Action Pawn"));
		return false;
	}

	APlayerController* PC = Cast<APlayerController>(Params.ActionController);

	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UActionType_IncreaseXP::PerformActionType() ActionController is not an APlayerController"));
		return false;
	}

	UTrpgControlComponent* ControlComp = UTurnBasedCombatLib::GetTrpgControlComp(PC);

	if (!ControlComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("UActionType_IncreaseXP::PerformActionType() Params.ActionController does not own a UInventoryControlComponent"));
		return false;
	}

	if (!Params.FocusedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("UActionType_IncreaseXP::PerformActionType() No FocusedActor"));
		return false;
	}

	AMockupFocusable* MockupFocusable = Cast<AMockupFocusable>(Params.FocusedActor);

	if (!MockupFocusable)
	{
		UE_LOG(LogTemp, Warning, TEXT("UActionType_IncreaseXP::PerformActionType() Params.FocusedActor is not a MockupFocusable"));
		return false;
	}

#pragma endregion

	ControlComp->AddXP(ControlComp->GetMainFighterProfile(), MockupFocusable);
	return true;
}

#undef LOCTEXT_NAMESPACE // "UActionType_IncreaseXP"