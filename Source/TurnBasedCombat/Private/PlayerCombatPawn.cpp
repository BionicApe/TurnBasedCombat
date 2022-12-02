// Created by Bionic Ape. All Rights Reserved.


#include "PlayerCombatPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "FocusInteractionsTypes.h"
#include "Components/StaticMeshComponent.h"
#include "Components/FocusTracerCursorComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/FighterComponent.h"
#include "Components/FocusTracerComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/SceneComponent.h"
#include "Combatant.h"
#include "Interfaces/BAUserOwner.h"
#include "BAMultiplayerSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Actor.h"
#include "TurnBasedCombatSubsystem.h"
#include "Fight.h"
#include "Arena.h"
#include "UI/StrategistHUD.h"
#include "GameFramework/HUD.h"
#include "Inventory/InventoryItem.h"
#include "FighterProfile.h"
#include "Components/InventoryComponent.h"
#include "TurnBasedCombatLog.h"
#include "Inventory/InventorySlot.h"
#include "Inventory/Inventory.h"
#include "ActionableType.h"
#include "Components/FocusableComponent.h"
#include "ActionsSet.h"
#include "Combat/ActionType_Trpg.h"
const FInputModeGameAndUI APlayerCombatPawn::GameModeAndUIInputMode = FInputModeGameAndUI().SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock).SetHideCursorDuringCapture(true);

APlayerCombatPawn::APlayerCombatPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	RootMeshComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootMeshComp);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootMeshComp);
	SpringArmComp->TargetArmLength = 750.f;
	SpringArmComp->bUsePawnControlRotation = false;
	SpringArmComp->bInheritRoll = false;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->bEnableCameraRotationLag = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->FieldOfView = 45.f;

	FocusTracer = CreateDefaultSubobject<UFocusTracerCursorComponent>(TEXT("FocusTracer"));
	FocusTracer->OnNewFocus.AddDynamic(this, &APlayerCombatPawn::OnNewFocus);
	FocusTracer->OnEndFocus.AddDynamic(this, &APlayerCombatPawn::OnEndFocus);
	FocusTracer->OnNewActionsSets.AddDynamic(this, &APlayerCombatPawn::OnNewActionsSets);

	bReplicates = true;//Epic says don't use SetReplicates in the constructor.
	SetReplicateMovement(false);

	//SetAutonomousProxy(true);
}

UInventoryItem* APlayerCombatPawn::GetCurrentInventoryItem()
{
	return CurrentInventoryItem;
}

void APlayerCombatPawn::SetCurrentInventoryItem(UInventoryItem* NewItem)
{
	CurrentInventoryItem = NewItem;
}

UInventoryItem * APlayerCombatPawn::GetDefaultInventoryItem()
{
	return DefaultInventoryItem;
}

void APlayerCombatPawn::Server_PerformLastAction_Implementation()
{
	if (!Combatant)
		return;
	FCombatantLastAction LastAction = Combatant->GetLastActionInfo();
	const UActionType* Action = Cast<UActionType>(LastAction.Action);
	if (Action)
	{
		SetSelectedItem(LastAction.Item);
		Fight->PerformAction(Action, this, Combatant, LastAction.Target);
	}
}

bool APlayerCombatPawn::CanPerformLastAction()
{
	if (!Combatant)
		return false;
	FCombatantLastAction LastAction = Combatant->GetLastActionInfo();
	if (LastAction.Action == nullptr)
		return false;
	SetSelectedItem(LastAction.Item);
	if (!LastAction.Action->CanExecuteAction(Combatant, LastAction.Target))
	{
		return false;
	}
	FTrpgPerformActionRequest Request;
	Request.Action = LastAction.Action;
	Request.Receiver = LastAction.Target;
	Request.Sender = Combatant;
	FTrpgPerformActionResult Result;
	return Combatant->ValidateAction(LastAction.Action, Request, Result);
}

void APlayerCombatPawn::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled() && TryToSetHUD())
	{
		StrategistHUD->ShowStartCombat();

	}
	


	//SetSelectedItem(Combatant->InventoryComp->GetInventory()->getinve);

}

void APlayerCombatPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCombatPawn, Combatant);
	DOREPLIFETIME(APlayerCombatPawn, bIsMyTurn);
	DOREPLIFETIME(APlayerCombatPawn, Profile);
	DOREPLIFETIME(APlayerCombatPawn, Fight);
	DOREPLIFETIME(APlayerCombatPawn, bHackIsCombatFinished);
	DOREPLIFETIME(APlayerCombatPawn, CurrentInventoryItem);
	//DOREPLIFETIME(APlayerCombatPawn, FocusTracer);
}

APlayerController* APlayerCombatPawn::GetPlayerController() const
{
	return Cast<APlayerController>(GetController());
}

void APlayerCombatPawn::BeginDestroy()
{
	//This crashes the engine
	//if (HasAuthority() && GetGameInstance())
	//{
	//	if (UTurnBasedCombatSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UTurnBasedCombatSubsystem>())
	//	{
	//		Subsystem->RemoveStrategist(this);
	//	}
	//}
	if (DefaultInventoryItem)
	{
		if (Combatant && Combatant->InventoryComp)
		{
			Combatant->InventoryComp->Inventory->Remove(GetDefaultInventoryItem(), 1, 0);
		}
	}
	Super::BeginDestroy();
}


#pragma region Input

// Called to bind functionality to input
void APlayerCombatPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("TurnRightMouse", this, &APlayerCombatPawn::Turn);
	PlayerInputComponent->BindAxis("TurnRightGamepad", this, &APlayerCombatPawn::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpMouse", this, &APlayerCombatPawn::LookUp);
	PlayerInputComponent->BindAxis("LookUpGamepad", this, &APlayerCombatPawn::LookUpAtRate);
	PlayerInputComponent->BindAxis("ZoomIn", this, &APlayerCombatPawn::ZoomIn);

	PlayerInputComponent->BindAction("TrpgMouse1", IE_Released, this, &APlayerCombatPawn::TrpgMouse1)/*.bConsumeInput = false*/;//I think the bConsumeInput was for the controller to pass down to the pawn, we don't need it here in the pawn
	PlayerInputComponent->BindAction("TrpgMouse2", IE_Released, this, &APlayerCombatPawn::TrpgMouse2)/*.bConsumeInput = false*/;//I think the bConsumeInput was for the controller to pass down to the pawn, we don't need it here in the pawn
}


void APlayerCombatPawn::Turn(float Rate)
{
	if (Rate != 0.f)
	{
		RootMeshComp->AddWorldRotation(FRotator(0.f, Rate * TurnSesibility, 0.f));
	}
}

void APlayerCombatPawn::TurnAtRate(float Rate)
{
	if (Rate != 0.f)
	{
		RootMeshComp->AddWorldRotation(FRotator(0.f, Rate * TurnRate * TurnSesibility * GetWorld()->GetDeltaSeconds(), 0.f));
	}
}


void APlayerCombatPawn::LookUp(float Rate)
{
	if (Rate != 0.f)
	{
		FRotator NewRotator = GetActorRotation();
		NewRotator.Pitch = FMath::ClampAngle(NewRotator.Pitch + Rate * LookUpSesibility, -45.f, 45.f);
		SetActorRotation(NewRotator);
	}
}

void APlayerCombatPawn::LookUpAtRate(float Rate)
{
	if (Rate != 0.f)
	{
		FRotator NewRotator = GetActorRotation();
		NewRotator.Pitch = FMath::ClampAngle(NewRotator.Pitch + Rate * LookUpSesibility * LookUpRate, -45.f, 45.f);
		SetActorRotation(NewRotator);
	}
}

void APlayerCombatPawn::ZoomIn(float Rate)
{
	if (FMath::Abs(Rate) > 0)
	{
		SpringArmComp->TargetArmLength = FMath::Clamp(Rate * ZoomRate + SpringArmComp->TargetArmLength, MinZoom, MaxZoom);
	}
}

void APlayerCombatPawn::TrpgMouse1()
{
	FFocusPerformAction FocusPerformAction;

	//FocusTracer->PerformAction(0);

	//TODO Ponerlo bonito
	if ( (StrategistHUD || TryToSetHUD()) && Combatant->IsMyTurn())
	{

		//FFocusTraceInfo f = FocusTracer->FocusTraceInfo;
		StrategistHUD->ShowActionsMenu(FocusTracer->FocusTraceInfo);

	}

	//if (FocusTracer->GetPerformActionWithIndex(0, FocusPerformAction))
	//{
	//	//Server call

	//	Server_PerformAction(FocusPerformAction);
	//	/*FocusPerformAction.ActionType->PerformActionType(FocusPerformAction);
	//	FTrpgPerformActionRequest Request;
	//	Request.Action = FocusPerformAction.ActionType;
	//	Request.Sender = Warrior;
	//	Request.Receiver = GetFocusedWarrior();
	//	Warrior->Fight->PerformAction(Request);*/
	//}
}

void APlayerCombatPawn::TrpgMouse2()
{
	if ( (StrategistHUD || TryToSetHUD()) && Combatant->IsMyTurn())
	{
		StrategistHUD->ShowMenu(Combatant);
	}
}

#pragma endregion

#pragma region FocusTracer

void APlayerCombatPawn::OnNewFocus(const FFocusTraceInfo& Info)
{
	if (!Combatant || !Combatant->IsMyTurn())
		return;
	//SetMouseCursorWidget(EMouseCursor::Crosshairs, Widget);//Here's another way to create a widget on the fly
	if (APlayerController* PC = GetPlayerController())
	{
		PC->CurrentMouseCursor = EMouseCursor::Crosshairs;

		if (AActor* CurrentFocusedActor = FocusTracer->GetFocusedActor())
		{
			FFocusPerformAction FocusPerformAction;
			bool value = FocusTracer->GetPerformActionWithIndex(0, FocusPerformAction);

			if (value && FocusPerformAction.FocusedActor == CurrentFocusedActor)
			{
				if (Fight->Arena)
				{
					Fight->Arena->SetSelectedActor(CurrentFocusedActor);
				}
			}
		}
	}
}

void APlayerCombatPawn::OnEndFocus(const UFocusableComponent* Focusable)
{
	if (APlayerController* PC = GetPlayerController())
	{
		PC->CurrentMouseCursor = EMouseCursor::Default;
		if (Fight->Arena)
		{
			Fight->Arena->RemoveSelectedActors();
		}
	}
}

void APlayerCombatPawn::OnNewActionsSets()
{
	
}

#pragma endregion


void APlayerCombatPawn::StartFight(AFight* NewFight)
{
	Fight = NewFight;
	SetActorTransform(Fight->Arena->GetOrbitCameraTransform());
	//APlayerController* PC = GetPlayerController();
	//if(!PC)
	//	PC = UBAMultiplayerSubsystem::GetPlayerControllerFromUser(this, Profile->BAUser);
	//if (PC)
	//{
	//	PC->Possess(this);
	//}
}

void APlayerCombatPawn::OnRep_Fight()
{
	if (!Fight)
		return;
	SetActorTransform(Fight->Arena->GetOrbitCameraTransform());
	if (IsLocallyControlled() && (StrategistHUD || TryToSetHUD()))
	{
		StrategistHUD->ShowStartCombat();
	}
}
void APlayerCombatPawn::StartTurn(UBAProfile* NewProfile, ACombatant* NewCombatant)
{
	Configure(NewProfile, NewCombatant);
	bIsMyTurn = true;
}

void APlayerCombatPawn::Configure(UBAProfile* NewProfile, ACombatant* NewCombatant)
{
	//if (GetDefaultInventoryItem())
	//{
	//	if (Combatant && Combatant->InventoryComp)
	//	{
	//		Combatant->InventoryComp->Inventory->Remove(GetDefaultInventoryItem(), 1, 0);
	//	}
	//}
	Profile = NewProfile;


	if(Combatant)
		Combatant->OnDefaultItemSelected.RemoveDynamic(this, &APlayerCombatPawn::Server_OnCombatantChangeItem);
	//Combatant
	Combatant = NewCombatant;
	Combatant->OnDefaultItemSelected.AddUniqueDynamic(this, &APlayerCombatPawn::Server_OnCombatantChangeItem);
	if (GetDefaultInventoryItem() && Combatant)
	{
		Combatant->Server_SetDefaultInventoryItem(GetDefaultInventoryItem());
		SetSelectedItem(GetDefaultInventoryItem());
	}
}

bool APlayerCombatPawn::ValidateAction(const UActionType* Action)
{
	//TODO mirar que el objeto que se esta usando tiene esa accion.
	bool ReturnedValue = false;
	//if (Combatant)
	//{
	//	ReturnedValue = Combatant->InventoryComp->SelectedItem->ActionsSet.Contains(Action);
	//}
	return ReturnedValue;
}



void APlayerCombatPawn::EndTurn()
{
	bIsMyTurn = false;
}

void APlayerCombatPawn::NotifyFightFinish(AFight* FinishedFight)
{
	Client_NotifyFightFinish();
}



void APlayerCombatPawn::Client_NotifyFightFinish_Implementation()
{
	OnCombatFinished.Broadcast();
	bHackIsCombatFinished = true;

	if (IsLocallyControlled() && (StrategistHUD || TryToSetHUD()))
	{
		if (Fight)
		{
			StrategistHUD->ShowFightResults(Fight->FightResults);
		}
	}
}

void APlayerCombatPawn::OnRep_Combatant()
{
	OnCombatantReady.Broadcast(Combatant);
}

void APlayerCombatPawn::OnRep_IsMyTurn()
{
	if (IsLocallyControlled() && (StrategistHUD || TryToSetHUD()))
	{
		if (bIsMyTurn)
		{
			StrategistHUD->ShowStartTurn();
		}
	}
}

void APlayerCombatPawn::OnRep_CurrentInventoryItem()
{
	OnItemChange.Broadcast(CurrentInventoryItem);
}

void APlayerCombatPawn::Server_OnCombatantChangeItem_Implementation(UInventoryItem* InventoryItem)
{
	if (InventoryItem == nullptr)
		return;
	FocusTracer->SetNewActionsSetDeleteOlds(InventoryItem->ActionsSet);
	SetCurrentInventoryItem(InventoryItem);
}

void APlayerCombatPawn::SetSelectedItem(UInventoryItem* InventoryItem)
{
	if (Combatant && GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		Combatant->InventoryComp->SetSelectedItem(InventoryItem);//Local
	}
	Server_SetSelectedItem(InventoryItem);
	//ENetRole r = GetLocalRole();

}


void APlayerCombatPawn::Server_SetSelectedItem_Implementation(UInventoryItem* InventoryItem)
{
	//if (FocusTracer->ActionsSets.IsEmpty())
	//{
	//}
	if (Combatant)
	{
		if (Combatant->InventoryComp->SetSelectedItem(InventoryItem))
		{
			FocusTracer->SetNewActionsSetDeleteOlds(InventoryItem->ActionsSet);
			//ActionsSet->Action->SetActionsSets(InventoryItem->ActionsSet);
			//for (UActionsSet* ActionSet : InventoryItem->ActionsSet)
			//{
			//	FocusTracer->SetNewActionsSetDeleteOlds(ActionSet);
			//}
			SetCurrentInventoryItem(InventoryItem);
		}
	}
}

bool APlayerCombatPawn::Server_SetSelectedItem_Validate(UInventoryItem* InventoryItem)
{
	return true;
}


void APlayerCombatPawn::RequestFinishTurn()
{
	Server_RequestFinishTurn();
}

void APlayerCombatPawn::Server_RequestFinishTurn_Implementation()
{
	Fight->FinishTurn(this, Combatant);
}

bool APlayerCombatPawn::Server_RequestFinishTurn_Validate()
{
	return true;
}

UInventory* APlayerCombatPawn::GetInventory() const
{
	if (UFighterProfile* FighterProfile = Cast<UFighterProfile>(Profile))
	{
		return FighterProfile->GetInventory();
	}
	return nullptr;
}

void APlayerCombatPawn::GetInventoryItemsList(TArray<UInventoryItem*>& MyInventoryItems) const
{
	if (UFighterProfile* FighterProfile = Cast<UFighterProfile>(Profile))
	{
		if (FighterProfile->GetInventory())
		{
			FighterProfile->GetInventory()->GetInventoryItemsList(MyInventoryItems);
		}
	}
}

bool APlayerCombatPawn::TryToSetHUD()
{
	if (APlayerController* PC = GetPlayerController())
	{
		StrategistHUD = Cast<IStrategistHUD>(PC->GetHUD());
	}

	return StrategistHUD != nullptr;
}