// Created by Bionic Ape. All Rights Reserved.


#include "Fight.h"
#include "TurnBasedCombat.h"
#include "Net/UnrealNetwork.h"
#include "Fighter.h"
#include "Components/FighterComponent.h"
#include "Team.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/TurnBasedStrategist.h"
#include "Combatant.h"
#include "Arena.h"
#include "TurnBasedCombatSubsystem.h"
#include "Combat/ActionType_Trpg.h"
#include "TrpgCombatTypes.h"
#include "BAProfile.h"
#include "Containers/Map.h"


// Sets default values
AFight::AFight() : Super()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//SetReplicates(true);//Directly setting bReplicates is the correct procedure for pre-init actors
	bReplicates = true;
	SetReplicateMovement(false);
	bAlwaysRelevant = true;
	//if (!CombatantClass)
	//{
	//	LoadConfig();//Load config from file
	//}
	static ConstructorHelpers::FClassFinder<ACombatant> CombatantClassRef(TEXT("/Game/ThePrison/Blueprints/TurnBasedCombat/BP_ThePrisonCombatant.BP_ThePrisonCombatant_C"));
	CombatantClass = CombatantClassRef.Class;
}

void AFight::BeginPlay()
{
	Super::BeginPlay();
	//if (HasAuthority())
	//{
	//	FTimerDelegate TimerCallback;
	//	TimerCallback.BindLambda([this]
	//		{
	//			StartFight();
	//		});
	//	FTimerHandle Handle;
	//	GetWorld()->GetTimerManager().SetTimer(Handle, TimerCallback, 2.f, false);
	//}

	FightState = EFightState::INITIALIZING;
}

void AFight::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFight, Teams);
	DOREPLIFETIME(AFight, Arena);
	DOREPLIFETIME(AFight, Turns);
	DOREPLIFETIME(AFight, TurnIndex);
}

void AFight::AddTeam(UTeam* Team)
{
	Teams.Add(Team);
}

bool AFight::IsCombatantTurn(ACombatant* Combatant) const
{
	if (Turns.IsValidIndex(TurnIndex))
	{
		return Turns[TurnIndex].Combatant == Combatant;
	}
	return false;
}

void AFight::StartFight()
{
	Combatants.Empty();

	if (!Arena)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("AFight::StartFight(): Arena is null"));
		return;
	}

	if (Teams.Num() < 2)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("AFight::StartFight(): Not enough Teams (Teams = %i)"), Teams.Num());
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("AFight::StartFight(): World is null"));
		return;
	}

	if (!CombatantClass)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("AFight::StartFight(): CombatantClass is null"));
		return;
	}

	UTurnBasedCombatSubsystem* TurnBasedCombatSubsystem = GetGameInstance()->GetSubsystem<UTurnBasedCombatSubsystem>();
	if (!TurnBasedCombatSubsystem)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("AFight::StartFight(): TurnBasedCombatSubsystem is null"));
		return;
	}

	int32 TeamIndex = 0;
	int32 CombatantIndex = 0;
	for (UTeam* Team : Teams)
	{
		CombatantIndex = 0;

		for (UBAProfile* Profile : Team->Profiles)
		{
			//Spawn A new Combatant
			ACombatant* Combatant = GetWorld()->SpawnActor<ACombatant>(CombatantClass, Arena->GetPawnTransform(TeamIndex, CombatantIndex));
			Combatant->SetBAProfile(Profile);
			Combatants.Add(Profile, Combatant);
			Combatant->Fight = this;

			TurnBasedCombatSubsystem->ProfileStartFight(Profile, this);

			int32 const NewTurnIndex = Turns.Emplace();
			FFightTurn& Turn = Turns[NewTurnIndex];
			Turn.Combatant = Combatant;
			Turn.Profile = Profile;
			Turn.Strategist = TurnBasedCombatSubsystem->GetStrategist(Profile);
			CombatantIndex++;
		}

		TeamIndex++;
	}

	//Test Turns

	FightState = EFightState::FIGHTING;
	SetTurnState(ETurnState::CAN_RECEIVE_ACTIONS);

	TurnIndex = -1;
	NotifyNextTurn();
}

void AFight::TestDelayNotifyTurn()
{
	FFightTurn& CurrentTurn = Turns[TurnIndex];

	if (CurrentTurn.Strategist)
	{
		CurrentTurn.Strategist->EndTurn();
	}


	TurnIndex = (TurnIndex + 1) % Turns.Num();
	FFightTurn& NewTurn = Turns[TurnIndex];

	if (NewTurn.Strategist)
	{
		NewTurn.Strategist->StartTurn(NewTurn.Profile, NewTurn.Combatant);
	}

	//if (GetWorld())
	//{
	//	FTimerHandle Handle;
	//	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AFight::TestDelayNotifyTurn, 5.f, false);
	//}
}

void AFight::NotifyNextTurn()
{
	if (Turns.Num() == 0)
	{
		UE_LOG(LogTurnBasedCombat, Log, TEXT("NotifyNextTurn Doesn't have a turn"));
		return;
	}

	TurnIndex = (TurnIndex + 1) % Turns.Num();
	FFightTurn& NewTurn = Turns[TurnIndex];

	if (NewTurn.Combatant)
	{
		NewTurn.Combatant->StartTurnUpdateValues();
	}
	if (NewTurn.Strategist)
	{
		NewTurn.Strategist->StartTurn(NewTurn.Profile, NewTurn.Combatant);
	}
}


bool AFight::AreEnemies(UBAProfile* ProfileA, UBAProfile* ProfileB) const
{
	if (!ProfileA || !ProfileB)
	{
		return false;// If one is null we can't say that are enemies
	}

	if (ProfileA == ProfileB)
	{
		return false;// they aren't enemies if they are the same profile
	}

	for (UTeam* Team : Teams)
	{
		if (Team->Profiles.Contains(ProfileA) && Team->Profiles.Contains(ProfileB))
		{
			return false;//They share the same team
		}
	}
	return true;
}

void AFight::SetTurnState(ETurnState NewTurnState)
{
	TurnState = NewTurnState;
}

bool AFight::PerformAction(UActionType* Action, ITurnBasedStrategist* Strategist, ACombatant* Sender, ACombatant* Receiver)
{
	//VALIDATIONS! TODO: MAYBE IT'S GOOD TO CHANGE THE FLOW
#pragma region Validations

	if (TurnState != ETurnState::CAN_RECEIVE_ACTIONS)
	{
		UE_LOG(LogTurnBasedCombat, Log, TEXT("Server_PerformAction_Implementation Can't receive Actions, TurnsState != ETurnState::CAN_RECEIVE_ACTIONS"));
		return false;
	}

	if (!Action)
	{
		UE_LOG(LogTurnBasedCombat, Log, TEXT("Server_PerformAction_Implementation has no PerformAction.ActionType"));
		return false;
	}

	if (!Strategist)
	{
		UE_LOG(LogTurnBasedCombat, Log, TEXT("It is not our combatant's turn at the moment"));
		return false;
	}

	if (!Sender)
	{
		UE_LOG(LogTurnBasedCombat, Log, TEXT("Sender is null"));
		return false;
	}

	if (!Receiver)
	{
		UE_LOG(LogTurnBasedCombat, Log, TEXT("Receiver is null"));
		return false;
	}

	if (!IsCombatantTurn(Sender))
	{
		UE_LOG(LogTurnBasedCombat, Log, TEXT("It is not our combatant's turn at the moment"));
		return false;
	}

	if (!Turns.IsValidIndex(TurnIndex))
	{
		//The Turn Index is Wrong
		UE_LOG(LogTurnBasedCombat, Log, TEXT("The Turn index is wrong"));
		return false;
	}

	UTurnBasedCombatSubsystem* TurnBasedCombatSubsystem = GetGameInstance()->GetSubsystem<UTurnBasedCombatSubsystem>();
	if (!TurnBasedCombatSubsystem)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("AFight::StartFight(): TurnBasedCombatSubsystem is null"));
		return false;
	}

	if (Strategist != TurnBasedCombatSubsystem->GetStrategist(Turns[TurnIndex].Profile))
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("The Strategists aren't the same"));
		return false;
	}

	//END VALIDATIONS, we only need to do the Action validations themselves

#pragma endregion



	if (UActionType_Trpg* TrpgAction = Cast<UActionType_Trpg>(Action))
	{
		FTrpgPerformActionRequest Request;
		Request.Action = TrpgAction;
		Request.Sender = Sender;
		Request.Receiver = Receiver;

		FTrpgPerformActionResult Result;

		if (!TrpgAction->Validate(Request, Result))
		{
			OnCombatLog.Broadcast(Result.Log);
			return false;
		}

		SetTurnState(ETurnState::PERFORMING_ACTION_STAGE);

		//We save the latest action so we can repeat it
		//Turns[TurnIndex].LastActionPerformed = TrpgAction;

		float SequenceDuration = Arena->PerformActionSequence(Action, Sender, Receiver);


		if (SequenceDuration > 0.f)
		{
			FTimerDelegate TimerCallback;
			TimerCallback.BindLambda([this, Request]
				{
					CommitAction(Request);
				});

			FTimerHandle Handle;
			GetWorld()->GetTimerManager().SetTimer(Handle, TimerCallback, SequenceDuration, false);
		}
		else
		{
			CommitAction(Request);
		}
	}

	return false;
}

void AFight::CommitAction(FTrpgPerformActionRequest Request)
{
	FTrpgPerformActionResult Result;

	if (UActionType_Trpg* Action = Cast<UActionType_Trpg>(Request.Action))
	{
		if (ACombatant* SenderCombatant = Cast < ACombatant>(Request.Sender))
		{
			Action->PerformAction(Request, Result);
			PayAction(Action, SenderCombatant);
		}
	}

	if (CheckCombatHasFinished())
	{
		for (UTeam* Team : Teams)
		{
			for (UBAProfile* Profile : Team->Profiles)
			{
				if (UTurnBasedCombatSubsystem* TurnBasedCombatSubsystem = GetGameInstance()->GetSubsystem<UTurnBasedCombatSubsystem>())
				{
					if (ITurnBasedStrategist* Strategist = TurnBasedCombatSubsystem->GetStrategist(Profile))
					{
						if (Strategist)
						{
							Strategist->NotifyFightFinish(this);


						}
					}
				}
			}
		}
		// Use a dummy timer handle as we don't need to store it for later but we don't need to look for something to clear
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFight::TerminateFight, WaitTimeToTerminate, false);
	}
	else// Combat is not finished 
	{
		SetTurnState(ETurnState::CAN_RECEIVE_ACTIONS);

		if (FFightTurn const* FFightTurn = GetCurrentFightTurn())
		{
			if (FFightTurn->Combatant && (FFightTurn->Combatant->ActionPoints <= 0 || FFightTurn->Combatant->RemainingActions <= 0))
			{
				NotifyNextTurn();
			}
		}
	}
}

void AFight::FinishTurn(ITurnBasedStrategist* Strategist, ACombatant* Combatant)
{
	if (!Turns.IsValidIndex(TurnIndex))
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("AFight::FinishTurn(): TurnIndex is an invalid index"));
		return;
	}

	FFightTurn& CurrentTurn = Turns[TurnIndex];

	if (CurrentTurn.Combatant != Combatant)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("AFight::FinishTurn(): Combatant is not correct"));
		return;
	}
	if (CurrentTurn.Strategist != Strategist)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("AFight::FinishTurn(): Strategist is not correct"));
		return;
	}

	CurrentTurn.Combatant->EndTurnUpdateValues();
	CurrentTurn.Strategist->EndTurn();
	NotifyNextTurn();
}

bool AFight::PayAction(UActionType_Trpg const* TrpgAction, ACombatant* Combatant)
{
	bool const bHasEnoughActionPoints = Combatant->ActionPoints >= TrpgAction->GetActionPoints();
	bool const bHastEnoughRemainingActions = Combatant->RemainingActions > 0;

	if (bHasEnoughActionPoints && bHastEnoughRemainingActions)
	{
		Combatant->ActionPoints -= TrpgAction->GetActionPoints();
		Combatant->RemainingActions--;
		return true;
	}

	return false;
}

bool AFight::CheckCombatHasFinished()
{
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		if (FightState == EFightState::FIGHTING)
		{
			TArray<UTeam*> TeamsAlive;
			for (UTeam* Team : Teams)
			{
				bool bIsTeamAlive = false;

				for (UBAProfile* Profile : Team->Profiles)
				{
					if (Combatants.Contains(Profile))
					{
						if (!Combatants[Profile]->IsDead())
						{
							bIsTeamAlive = true;
						}
					}
				}

				if (bIsTeamAlive)
				{
					TeamsAlive.Add(Team);
				}
			}//End  for(UTeam* Team : Teams)

			if (TeamsAlive.Num() <= 1)
			{
				FightResults.Fight = this;
				FightResults.ResultType = TeamsAlive.Num() == 0 ? EFightResultType::ALL_DEFEATED : EFightResultType::HAS_A_WINNER;
				FightResults.WinnerTeam = TeamsAlive.Num() > 0 ? TeamsAlive[0] : nullptr;
				if (FightResults.WinnerTeam)
				{
					FightResults.WinnerTeamName = FightResults.WinnerTeam->Profiles[0]->ProfileName;
				}
				FightState = EFightState::FINISHED;
			}
		}
	}

	return FightState == EFightState::FINISHED;//
}

void AFight::TerminateFight()
{
	UTurnBasedCombatSubsystem* TurnBasedCombatSubsystem = GetGameInstance()->GetSubsystem<UTurnBasedCombatSubsystem>();

	for (auto CombatantIt = Combatants.CreateIterator(); CombatantIt; ++CombatantIt)
	{
		if (ACombatant* Combatant = CombatantIt.Value())
		{
			bool const IsDead = Combatant->IsDead();

			FTransform CurrentTransform = Combatant->GetActorTransform();


			if (UBAProfile* Profile = Cast<UBAProfile>(CombatantIt.Key()))
			{
				TurnBasedCombatSubsystem->StartExplorationMode(Profile, this, IsDead, CurrentTransform);
			}
			Combatant->Destroy();
		}
	}
	//I commented this because I'm going to destroy this actor
	//Combatants.Reset();
	//Teams.Empty();

	if (Arena)
	{
		Arena->Reset();
	}

	Destroy();
}