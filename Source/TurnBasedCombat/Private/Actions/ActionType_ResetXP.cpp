//// Created by Bionic Ape. All Rights Reserved.
//
//
//#include "Actions/ActionType_ResetXP.h"
//#include "FocusInteractionsTypes.h"
//#include "GameFramework/Actor.h"
//#include "GameFramework/PlayerController.h"
//#include "GameFramework/HUD.h"
//#include "Components/InventoryControlComponent.h"
//#include "InventoryFunctionLibrary.h"
//#include "MockupCoinGenerator.h"
//#include "Interfaces/InventoryHUDInterface.h"
//
//#define LOCTEXT_NAMESPACE "UActionType_ResetXP"
//
//bool UActionType_ResetXP::CanExecuteAction(AActor* ActionActor, AActor* ActionableActor) const
//{
//	return true;
//}
//
//bool UActionType_ResetXP::PerformActionType(FFocusPerformAction Params) const
//{
//
//#pragma region ErrorChecking
//	UE_LOG(LogTemp, Warning, TEXT("UActionType_ResetXP::PerformActionType()"));
//
//
//	if (!Params.ActionController)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("UActionType_ResetXP::PerformActionType() No Action Pawn"));
//		return false;
//	}
//
//	APlayerController* PC = Cast<APlayerController>(Params.ActionController);
//
//	if (!PC)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("UActionType_ResetXP::PerformActionType() ActionController is not an APlayerController"));
//		return false;
//	}
//
//	UInventoryControlComponent* InventoryControlComp = UInventoryFunctionLibrary::GetInventoryControlComp(PC);
//
//	if (!InventoryControlComp)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("UActionType_ResetXP::PerformActionType() Params.ActionController does not own a UInventoryControlComponent"));
//		return false;
//	}
//
//	if (!Params.FocusedActor)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("UActionType_ResetXP::PerformActionType() No FocusedActor"));
//		return false;
//	}
//
//	AMockupCoinGenerator* CoinGenerator = Cast<AMockupCoinGenerator>(Params.FocusedActor);
//
//	if (!CoinGenerator)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("UActionType_ResetXP::PerformActionType() Params.FocusedActor is not a CoinGenerator"));
//		return false;
//	}
//
//#pragma endregion
//
//	//CoinGenerator->
//
//	InventoryControlComp->ModifyCoins(InventoryControlComp->GetMainInventory(), CoinGenerator);
//
//	return true;
//}
//
//#undef LOCTEXT_NAMESPACE // "UActionType_ResetXP"