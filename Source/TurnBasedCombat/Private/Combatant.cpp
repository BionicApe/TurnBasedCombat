// Created by Bionic Ape. All Rights Reserved.

#include "Combatant.h"
#include "Components/HealthComponent.h"
#include "BAProfile.h"
#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/HealthComponent.h"
#include "Components/FocusableComponent.h"
#include "FighterProfile.h"
#include "Inventory/InventoryItem.h"
#include "ActionsSet.h"
#include "Combat/ActionType_Trpg.h"
#include "ActionsSet.h"
#include "Fight.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Inventory/Inventory.h"

#include "BARPGPersona.h"
//#include "BARPG/ThePrisonPersona.h"


#define LOCTEXT_NAMESPACE "ACombatant"
// Sets default values
ACombatant::ACombatant()
{
	//if (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer)
	//{
	//	PrimaryActorTick.bCanEverTick = true;
	//	PrimaryActorTick.bStartWithTickEnabled = true;
	//	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	//}
	//else
	//{
	//	PrimaryActorTick.bCanEverTick = false;
	//	PrimaryActorTick.bStartWithTickEnabled = true;
	//}
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = false;
	//SetReplicates(true);//Directly setting bReplicates is the correct procedure for pre-init actors
	bReplicates = true;
	SetReplicatingMovement(false);

	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComp"));
	InventoryComp->SetIsReplicated(true);
	InventoryComp->OnNewItemSelected.AddDynamic(this, &ACombatant::OnNewItemSelected);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComp->OnDieEvent.AddDynamic(this, &ACombatant::OnDie);
	HealthComp->OnTakeDamageEvent.AddDynamic(this, &ACombatant::OnTakeDamage);
	HealthComp->SetIsReplicated(true);

	FocusableComp = CreateDefaultSubobject<UFocusableComponent>(TEXT("FocusableComp"));
	//ID = FGuid::NewGuid();
}

void ACombatant::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//Only executed on client
	if (bIsMyTurn && GetWorld() && !PerformingAction)
	{
		RemainingSeconds = RemainingSeconds - DeltaSeconds ;
		OnRemainingSecondsChange.Broadcast(RemainingSeconds);
	}
}

//void ACombatant::BeginPlay()
//{
//	Super::BeginPlay();
//	GLog->Log("Hello World1");
//	SetActorTickEnabled(true);
//}

void ACombatant::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACombatant, Profile);
	DOREPLIFETIME(ACombatant, Fight);
	DOREPLIFETIME(ACombatant, ActionPoints);
	DOREPLIFETIME(ACombatant, RemainingActions);
	DOREPLIFETIME(ACombatant, bIsDead);
	DOREPLIFETIME(ACombatant, bIsMyTurn);
	DOREPLIFETIME(ACombatant, RemainingSeconds);
	DOREPLIFETIME(ACombatant, PerformingAction);
	DOREPLIFETIME(ACombatant, ID);
	DOREPLIFETIME(ACombatant, LastAction);
	DOREPLIFETIME(ACombatant, DefaultInventoryItem);
}

float ACombatant::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float ReturnValue = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HealthComp->TakeDamage(DamageAmount,DamageEvent,EventInstigator,DamageCauser);
	return ReturnValue;
}

//float ACombatant::Heal(float HealAmount)
//{
//	return HealthComp->Heal(HealAmount);
//}

void ACombatant::SetBAProfile(UBAProfile* NewProfile)
{
	Profile = Cast<UFighterProfile>(NewProfile);
	
	if (Profile)
	{
		InventoryComp->SetInventory(Profile->GetInventory());
		//InventoryComp->Inventory = Profile->GetInventory();
	}
}

UBAProfile* ACombatant::GetBAProfile() const
{
	return Profile;
}

void ACombatant::GenerateID()
{
	ID = FGuid::NewGuid();
}


void ACombatant::OnNewItemSelected(UInventoryItem* InventoryItem)
{
	
}

void ACombatant::StartTurnUpdateValues()
{
	bIsMyTurn = true;
	ActionPoints = Profile->GetActionPoints();
	RemainingActions = Profile->GetActionsPerTurn();
	RemainingSeconds = 40;
	//SetActorTickEnabled(true);
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TurnTimerHandle, this, &ACombatant::TimerEnds, RemainingSeconds, false);
	}
}

void ACombatant::EndTurnUpdateValues()
{	
	bIsMyTurn = false;
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TurnTimerHandle);
	}
}

void ACombatant::OnDie(FActorKilled ActorKilledProperties)
{
	bIsDead = true;
}

void ACombatant::OnTakeDamage(FTakeDamageProperties const TakeDamageProperties)
{

}

bool ACombatant::IsDead()
{
	return !HealthComp->IsAlive();
}

void ACombatant::OnRep_IsDead()
{
	if (bIsDead)
	{
		OnCombatantDie.Broadcast();
	}
}

void ACombatant::OnRep_ActionPoints()
{
	OnActionPointsChange.Broadcast(ActionPoints);
}

void ACombatant::OnRep_RemainingActions()
{
	OnRemainActionsChange.Broadcast(RemainingActions);
}

void ACombatant::OnRep_Fight()
{
	OnFightSetUp.Broadcast(Fight);
}

void ACombatant::OnRep_IsMyTurn()
{
	OnTurnUpdate.Broadcast(bIsMyTurn);
}

void ACombatant::OnRep_RemainingSeconds()
{
	OnRemainingSecondsChange.Broadcast(RemainingSeconds);
}

void ACombatant::TimerEnds()
{
	if (!bIsMyTurn)
		return;
	RemainingSeconds = 0;//sync with clients (If we don't change the value UE is not sync the data)
	Fight->NotifyNextTurn();
}

void ACombatant::StartPerformingAction(const UActionType_Trpg* Action, ACombatant* Target)
{
	PerformingAction = true;
	GetWorld()->GetTimerManager().PauseTimer(TurnTimerHandle);
	RemainingSeconds = GetWorld()->GetTimerManager().GetTimerRemaining(TurnTimerHandle);//For sync
	LastAction.Action = Action;
	LastAction.Target = Target;
	LastAction.Item = InventoryComp->SelectedItem;
	OnCombatantStartAction.Broadcast(LastAction);
}

void ACombatant::EndPerformingAction()
{
	//RemainingSeconds = GetWorld()->GetTimerManager().GetTimerRemaining(TurnTimerHandle);//For sync
	GetWorld()->GetTimerManager().UnPauseTimer(TurnTimerHandle);
	PerformingAction = false;
}

void ACombatant::SubstractActionPoints(unsigned int Value)
{
	ActionPoints = FGenericPlatformMath::Max(ActionPoints-Value, 0u);
}

void ACombatant::SubstractActions(unsigned int Value)
{
	RemainingActions = FGenericPlatformMath::Max(RemainingActions-Value, 0u);
}

void ACombatant::Server_SetDefaultInventoryItem_Implementation(UInventoryItem* NewDefaultInventoryItem)
{
	if (DefaultInventoryItem)
	{
		if (InventoryComp)
		{
			InventoryComp->Inventory->Remove(DefaultInventoryItem, 1, 0);
		}
	}
	if (NewDefaultInventoryItem)
	{
		if (InventoryComp)
		{
			InventoryComp->Inventory->Add(NewDefaultInventoryItem, 1, 0);
		}
		DefaultInventoryItem = NewDefaultInventoryItem;
	}
}

FCombatantLastAction ACombatant::GetLastActionInfo()
{
	return LastAction;
}

bool ACombatant::CanUseThisItem(UInventoryItem* Item)
{
	if (Item->ActionsSet.IsEmpty())
		return false;

	for (UActionsSet* ActionSet : Item->ActionsSet)
	{
		TArray<UActionType*> Actions = ActionSet->GetAllActions();
		for (UActionType* Action : Actions)
		{
			UActionType_Trpg* ActionRPG = Cast<UActionType_Trpg>(Action);
			if (ActionRPG && (unsigned int)ActionRPG->GetActionPoints() <= ActionPoints)
			{
				return true;
			}
		}
	}

	return false;
}

bool ACombatant::HasEnoughActionPoints(unsigned int Value)
{
	return ActionPoints >= Value;
}

bool ACombatant::HasEnoughActions()
{
	return RemainingActions > 0;
}

bool ACombatant::TryConsumeActionPoints(unsigned int Value)
{
	if (ActionPoints < Value)
		return false;
	ActionPoints -= Value;
	//OnActionPointsChange.Broadcast(ActionPoints);
	return true;
}

bool ACombatant::TryPerformAction()
{
	if (RemainingActions == 0)
		return false;

	RemainingActions--;
	return true;
}


bool ACombatant::ValidateAction(const UActionType_Trpg* Action, FTrpgPerformActionRequest Request, FTrpgPerformActionResult& Result)
{
	//Mirar que el objeto que se esta usando tiene esa accion.
	if (!Action->Validate(Request, Result))
		return false;

	bool ValidAction = false;
	if (InventoryComp->SelectedItem)
	{
		for (auto ActionSet : InventoryComp->SelectedItem->ActionsSet)
		{
			if (ActionSet->GetAllActions().Contains(Action))
			{
				ValidAction = true;
				break;
			}
		}
	}
	
	if (!ValidAction)
	{
		Result.Log.Messages.Add(LOCTEXT("Combatant","Not item equiped"));
	}
	bool EnoughAP = HasEnoughActionPoints(Action->GetActionPoints());
	if (!EnoughAP)
	{
		Result.Log.Messages.Add(LOCTEXT("Combatant", "Not enough action points"));
	}
	bool EnoughActions = HasEnoughActions();
	if (!EnoughActions)
	{
		Result.Log.Messages.Add(LOCTEXT("Combatant", "Not enough actions"));
	}
	return ValidAction && EnoughAP && EnoughActions;
}

void ACombatant::PayActionWithoutValidation(const UActionType_Trpg* Action)
{
	SubstractActionPoints(Action->GetActionPoints());
	SubstractActions(Action->GetActions());

	//Problema con las IAs, me han dicho que tendran su propio inventario y esto deberia funcionar. Ahora mismo no funciona ya que no seleccionan items ni nada.
	//Cuando esten bien implementadas las IAs elimiar el if
	if(InventoryComp->SelectedItem)
		InventoryComp->SelectedItem->Category->Use(InventoryComp->GetInventory(), InventoryComp->SelectedItem, GetWorld());
	
	if (DefaultInventoryItem && InventoryComp->SelectedItem == nullptr)
	{
		if (InventoryComp->SetSelectedItem(DefaultInventoryItem))
		{
			OnDefaultItemSelected.Broadcast(DefaultInventoryItem);
		}
		//else it is possible that we dont have the item?
	}

	//Si es un consumbale eliminar 1 de cantidad?
	// Poner en inventorycategory una funcion virtual para que gestione su propia logica de que hacer cuando se utiliza? Pasarle el inventario y no se que mas a esa funcion
	//if (InventoryComp->SelectedItem)
	//{

	//}
}

bool ACombatant::PayActionWithValidation(const UActionType_Trpg* Action, FTrpgPerformActionRequest Request, FTrpgPerformActionResult& Result)
{
	bool CanPerformAction = ValidateAction(Action, Request, Result);
	if (CanPerformAction)
	{
		PayActionWithoutValidation(Action);
	}
	return CanPerformAction;
}

#undef LOCTEXT_NAMESPACE