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
#include "TurnBasedCombat.h"
#include "Inventory/InventorySlot.h"
#include "Inventory/Inventory.h"

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

void APlayerCombatPawn::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled() && TryToSetHUD())
	{
		StrategistHUD->ShowStartCombat();
	}

}

void APlayerCombatPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCombatPawn, Combatant);
	DOREPLIFETIME(APlayerCombatPawn, bIsMyTurn);
	DOREPLIFETIME(APlayerCombatPawn, Profile);
	DOREPLIFETIME(APlayerCombatPawn, Fight);
	DOREPLIFETIME(APlayerCombatPawn, bHackIsCombatFinished);
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
	Super::BeginDestroy();
}


void APlayerCombatPawn::Restart()
{
	Super::Restart();
}

void APlayerCombatPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

#pragma region Input

// Called to bind functionality to input
void APlayerCombatPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APlayerCombatPawn::Turn);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCombatPawn::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCombatPawn::LookUp);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCombatPawn::LookUpAtRate);
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
	if (FocusTracer->GetPerformActionWithIndex(0, FocusPerformAction))
	{
		//Server call

		Server_PerformAction(FocusPerformAction);
		/*FocusPerformAction.ActionType->PerformActionType(FocusPerformAction);
		FTrpgPerformActionRequest Request;
		Request.Action = FocusPerformAction.ActionType;
		Request.Sender = Warrior;
		Request.Receiver = GetFocusedWarrior();
		Warrior->Fight->PerformAction(Request);*/
	}
}

void APlayerCombatPawn::TrpgMouse2()
{
	if (StrategistHUD || TryToSetHUD())
	{
		StrategistHUD->ShowMenu(Combatant);
	}
}

#pragma endregion

#pragma region FocusTracer
UFighterComponent* APlayerCombatPawn::GetFocusedFighterComponent() const
{
	if (AActor const* const FocusedActor = FocusTracer->GetFocusedActor())
	{
		if (UFighterComponent* FocusedFighterComponent = Cast<UFighterComponent>(FocusedActor->GetComponentByClass(UFighterComponent::StaticClass())))
		{
			return FocusedFighterComponent;
		}
	}
	return nullptr;
}


void APlayerCombatPawn::OnNewFocus(const FFocusTraceInfo& Info)
{
	//SetMouseCursorWidget(EMouseCursor::Crosshairs, Widget);//Here's another way to create a widget on the fly
	if (APlayerController* PC = GetPlayerController())
	{
		PC->CurrentMouseCursor = EMouseCursor::Crosshairs;

		if (AActor* CurrentFocusedActor = FocusTracer->GetFocusedActor())
		{
			if (UFighterComponent* FocusedWarrior = GetFocusedFighterComponent())
			{
				//Warrior->AICombatController->SetFocus(CurrentFocusedActor);

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

		if (UFighterComponent* FocusedWarrior = GetFocusedFighterComponent())
		{
			if (Fight->Arena)
			{
				Fight->Arena->RemoveSelectedActors();
			}
		}
	}
}

void APlayerCombatPawn::OnNewActionsSets()
{
	//if (AThePrisonHUD* MyThePrisonHUD = Cast<AThePrisonHUD>(GetHUD()))
	//{
	//	MyThePrisonHUD->OnNewActionsSets(FocusTracer);
	//}
}


void APlayerCombatPawn::Server_PerformAction_Implementation(FFocusPerformAction PerformAction)
{
	UE_LOG(LogTurnBasedCombat, Log, TEXT("Server_PerformAction_Implementation Received: %s"), *PerformAction.ToString());

	//All of this can go into _Validate()
	if (!Combatant)
	{
		UE_LOG(LogTurnBasedCombat, Log, TEXT("Server_PerformAction_Implementation has no Combatant"));
		return;
	}

	if (!Fight)
	{
		UE_LOG(LogTurnBasedCombat, Log, TEXT("Server_PerformAction_Implementation has no Fight"));
		return;
	}

	ACombatant* Receiver = Cast<ACombatant>(PerformAction.FocusedActor);
	if (!Receiver)
	{
		UE_LOG(LogTurnBasedCombat, Log, TEXT("Server_PerformAction_Implementation has no Combatant focused"));
		return;
	}

	bool const bIsActionPerformed = Fight->PerformAction(PerformAction.ActionType, this, Combatant, Receiver);
	UE_LOG(LogTurnBasedCombat, Log, TEXT("Server_PerformAction_Implementation Fight->PerformAction returned %s"), bIsActionPerformed ? TEXT("True") : TEXT("False"));
}

bool APlayerCombatPawn::Server_PerformAction_Validate(FFocusPerformAction PerformAction)
{
	//PerformAction.ActionsSet; //check if the actions set belongs to the 
	//PerformAction.ActionType;
	//PerformAction.ActionableType;
	//PerformAction.Focusable;
	//PerformAction.ActionPawn;
	//PerformAction.ActionController;
	//PerformAction.Distance;
	//PerformAction.FocusedActor;
	return true;
}
#pragma endregion


void APlayerCombatPawn::StartFight(AFight* NewFight)
{
	Fight = NewFight;
	SetActorTransform(Fight->Arena->GetOrbitCameraTransform());	
}

void APlayerCombatPawn::OnRep_Fight()
{
	SetActorTransform(Fight->Arena->GetOrbitCameraTransform());

	if (IsLocallyControlled() && (StrategistHUD || TryToSetHUD()))
	{
		StrategistHUD->ShowStartCombat();
	}
}
void APlayerCombatPawn::StartTurn(UBAProfile* NewProfile, ACombatant* NewCombatant)
{
	Combatant = NewCombatant;
	Profile = NewProfile;
	bIsMyTurn = true;
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

void APlayerCombatPawn::SetSelectedItem(UInventoryItem* InventoryItem)
{
	Server_SetSelectedItem(InventoryItem);
}


void APlayerCombatPawn::Server_SetSelectedItem_Implementation(UInventoryItem* InventoryItem)
{
	if (Combatant)
	{
		if (Combatant->InventoryComp->SetSelectedItem(InventoryItem))
		{
			FocusTracer->SetNewActionsSetDeleteOlds(InventoryItem->ActionsSet);
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
		return FighterProfile->Inventory;
	}
	return nullptr;
}

void APlayerCombatPawn::GetInventoryItemsList(TArray<UInventoryItem*>& MyInventoryItems) const
{
	if (UFighterProfile* FighterProfile = Cast<UFighterProfile>(Profile))
	{
		if (FighterProfile->Inventory)
		{
			FighterProfile->Inventory->GetInventoryItemsList(MyInventoryItems);
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