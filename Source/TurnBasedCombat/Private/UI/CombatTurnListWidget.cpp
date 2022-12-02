// Created by Bionic Ape. All Rights Reserved.


#include "UI/CombatTurnListWidget.h"
#include "Combatant.h"
#include "Fight.h"
#include "UI/CombatTurnListItemWidget.h"
#include "UI/CombatUITurnInfo.h"
#include "Components/ListView.h"
#include "PlayerCombatPawn.h"
#include "Components/HealthComponent.h"

bool UCombatTurnListWidget::Initialize()
{
	bool bResult = Super::Initialize();
	if (bResult)
	{
		//FinishTurnButton->OnClicked.AddDynamic(this, &UStrategistMenuWidget::OnFinishTurnButtonClicked);
		if (APlayerCombatPawn* MyPawn = GetPlayerCombatPawn())
		{
			if (MyPawn->Combatant)
			{
				ConfigureCombatantListeners(MyPawn->Combatant);
			}
			else
			{
				MyPawn->OnCombatantReady.AddUniqueDynamic(this, &UCombatTurnListWidget::ConfigureCombatantListeners);
			}
		}
	}

	return bResult;
}

void UCombatTurnListWidget::BeginDestroy()
{
	Super::BeginDestroy();
}

APlayerCombatPawn* UCombatTurnListWidget::GetPlayerCombatPawn() const
{
	return Cast<APlayerCombatPawn>(GetOwningPlayerPawn());
}

void UCombatTurnListWidget::ConfigureFightListeners(AFight* Fight)
{
	MyFight = Fight;
	NewRound(Fight->GetTurns());
	Fight->OnNewTurnCombatant.AddUniqueDynamic(this, &UCombatTurnListWidget::UpdateCurrentTurnCombatant); //Update turn
	Fight->OnNewRound.AddUniqueDynamic(this, &UCombatTurnListWidget::NewRound); //Update all info
	//Fight->OnCombatantDieOrRevive.AddUniqueDynamic(this, &UCombatTurnListWidget::UpdateCombatantDeadOrAlive);
	//Evento cuando alguien muere //Update dead
}

void UCombatTurnListWidget::ConfigureCombatantListeners(ACombatant* Combatant)
{
	MyCombatant = Combatant;
	if (Combatant->Fight)
	{
		ConfigureFightListeners(Combatant->Fight);
	}
	else
	{
		Combatant->OnFightSetUp.AddUniqueDynamic(this, &UCombatTurnListWidget::ConfigureFightListeners);
	}
}

void UCombatTurnListWidget::UpdateCurrentTurnCombatant(FFightTurn CombatantInfo)
{

	if (CreatingWindgets)
	{
		//UpdateTurnWileCreatingWidgets = true;
	//	CurrentTurn = CombatantInfo.CombatantInfo.Pos;
		return;
	}
	if (Widgets.IsEmpty())
		return;
	Widgets[CurrentTurn]->HiddeTurn();
	CurrentTurn = CombatantInfo.CombatantInfo.Pos;
	Widgets[CurrentTurn]->ShowTurn();
}

void UCombatTurnListWidget::NewRound(TArray<FFightTurn> Turns)
{
	if (Turns.IsEmpty())
		return;
	UpdateTurnWileCreatingWidgets = false; //Posiblemente sobre
	CreatingWindgets = true;
	if (!Widgets.IsEmpty())
		Widgets[CurrentTurn]->HiddeTurn();
	//Widgets.Empty();
	//TArray<UCombatUITurnInfo*> ActionsListInfo;

	Combatants.Empty();
	
	for(int j = 0; j< Turns.Num(); j++)
		Combatants.Add(Turns[j].CombatantInfo.ID.ToString(), Turns[j].Combatant);
	//Añadir listener de cuando cambia la vida y quitarlo del fight.
	for (auto CombatantPair : Combatants)
	{
		if (!CombatantPair.Value)
			continue;
		UHealthComponent* HealthComp = Cast<UHealthComponent>(CombatantPair.Value->GetComponentByClass(UHealthComponent::StaticClass()));
		if (HealthComp)
		{
			HealthComp->OnHealthChangedWithOwner.AddUniqueDynamic(this, &UCombatTurnListWidget::OnCombatantHealthChange);
		}
	}


	NCombatants = Turns.Num();

	//ItemsTileView reuse the previous widgets, so if it exist we have to update manually.
	WidgetsToLoad = Turns.Num();
	int i = 0;
	for (UObject* Item : TurnListView->GetListItems()) 
	{
		UUserWidget* NewWidget = TurnListView->GetEntryWidgetFromItem(Item);
		UCombatTurnListItemWidget* Widget = Cast<UCombatTurnListItemWidget>(NewWidget);
		if (Widget)
		{
			Widget->HiddeTurn();
			if (i < Turns.Num())
			{
				Widget->Info = UCombatUITurnInfo::NEW(Turns[i].CombatantInfo.CombatantName, i, Turns[i].CombatantInfo.ID.ToString());
				WidgetLoaded(*NewWidget);
				i++;
			}
			else
			{
				TurnListView->RemoveItem(Item);
			}
		}
	}
	for (; i < Turns.Num(); i++)
	{
		
		TurnListView->AddItem(UCombatUITurnInfo::NEW(Turns[i].CombatantInfo.CombatantName, i, Turns[i].CombatantInfo.ID.ToString()));
		//ActionsListInfo.Add(UCombatUITurnInfo::NEW(Turns[i].CombatantInfo.CombatantName, i,Turns[i].CombatantInfo.ID.ToString()));
	}
	
	//TurnListView->ClearListItems();
	//TurnListView->SetListItems(ActionsListInfo);

	TurnListView->OnEntryWidgetGenerated().AddUObject(this, &UCombatTurnListWidget::WidgetLoaded);

}

void UCombatTurnListWidget::WidgetLoaded(UUserWidget& NewWidget)
{
	UCombatTurnListItemWidget* Widget = Cast<UCombatTurnListItemWidget>(&NewWidget);
	if (Widget == nullptr || Widget->Info == nullptr)
		return;
	Widget->Configure();
	int Index = Widgets.Find(Widget);
	if (Index != INDEX_NONE)
	{
		Widgets.Swap(Index, Widget->Info->Pos);
	}
	else
	{
		Widgets.EmplaceAt(Widget->Info->Pos, Widget);
	}
	UpdateDeadOrAlive(Widget);
	WidgetsToLoad--;
	if (WidgetsToLoad == 0)
	{
		
		//if (UpdateTurnWileCreatingWidgets)
		//{
		//	if(MyFight)
		//		
		//}
		//UpdateTurnWileCreatingWidgets = false;
		CreatingWindgets = false;
		UpdateCurrentTurnCombatant(*MyFight->GetCurrentFightTurn());
		//UpdateCurrentTurnCombatant(*MyFight->GetCurrentFightTurn());
	}
	//CombatantWidget.Add(Widget->Info->ID, Widget);
}

void UCombatTurnListWidget::UpdateDeadOrAlive(UCombatTurnListItemWidget * Widget)
{
	if (Combatants.Contains(Widget->Info->ID))
	{
		ACombatant* Combatant = *Combatants.Find(Widget->Info->ID);
		if (Combatant != nullptr)
		{
			//bool d = Combatant->IsDead();
			Widget->SetDead(Combatant->IsDead());
		}
	}
}

//void UCombatTurnListWidget::UpdateCombatantDeadOrAlive(ACombatant* Combatant, bool IsDead)
//{
//	for (auto Widget : Widgets)
//	{
//		if (Widget->Info->ID == Combatant->ID.ToString())
//		{
//			bool d = Combatant->IsDead();
//			Widget->SetDead(IsDead);
//			break;
//		}
//	}
//}

void UCombatTurnListWidget::OnCombatantHealthChange(int32 OldHealth, int32 NewHealth, AActor* HealthOwner)
{
	ACombatant* Combatant = Cast<ACombatant>(HealthOwner);
	if (Combatant == nullptr)
		return;
	for (auto Widget : Widgets)
	{
		if (Widget->Info->ID == Combatant->ID.ToString())
		{
			bool d = Combatant->IsDead();
			Widget->SetDead(d);
			break;
		}
	}
}
