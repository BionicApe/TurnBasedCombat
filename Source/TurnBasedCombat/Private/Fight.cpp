// Created by Bionic Ape. All Rights Reserved.


#include "Fight.h"
#include "TurnBasedCombatLog.h"
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
#include "Components/HealthComponent.h"
#include <BAMultiplayerSubsystem.h>
#include <FighterProfile.h>
#include <Engine/World.h>

#include "TurnBasedCombatLog.h"


//For testing
#include "Kismet/KismetArrayLibrary.h"

// Sets default values
AFight::AFight() : Super()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;//Epic recommends to not use Set Replicates in Constructor "Directly setting bReplicates is the correct procedure for pre-init actors"
	SetReplicateMovement(false);
	bAlwaysRelevant = true;
}

void AFight::BeginPlay()
{
	Super::BeginPlay();
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

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("AFight::StartFight(): World is null"));
		return;
	}

	if (!Arena)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("AFight::StartFight(): Arena is null"));
		return;
	}

	if (!CombatantClass)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("AFight::StartFight(): CombatantClass is null"));
		return;
	}

	if (Teams.Num() < 2)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("AFight::StartFight(): Not enough Teams (Teams = %i)"), Teams.Num());
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

		for (UFighterProfile* Profile : Team->Profiles)
		{
			////Spawn A new Combatant
			//ACombatant* Combatant = GetWorld()->SpawnActor<ACombatant>(CombatantClass, Arena->GetPawnTransform(TeamIndex, CombatantIndex));
			////BAMultiplayerSpawnpawn o spawnactor
			//Combatant->SetBAProfile(Profile);
			//Combatants.Add(Profile, Combatant);
			//Combatant->Fight = this;
			//Combatant->GenerateID();

			const FTransform SpawnTransform = Arena->GetPawnTransform(TeamIndex, CombatantIndex);
			//Spawn A new Combatant
			ACombatant* Combatant = GetWorld()->SpawnActorDeferred<ACombatant>(CombatantClass, SpawnTransform);
			Combatant->SetBAProfile(Profile);
			Combatants.Add(Profile, Combatant);
			//TODO ADD Persona to profile
			Combatant->Fight = this;
			Combatant->GenerateID();
			Combatant->FinishSpawning(SpawnTransform);
			//UHealthComponent* HealthComp = Cast<UHealthComponent>(Combatant->GetComponentByClass(UHealthComponent::StaticClass()));
			//if (HealthComp)
			//{
			//	HealthComp->OnHealthChangedWithOwner.AddUniqueDynamic(this, &AFight::OnCombatantHealthChange);
			//}

			


			//CombatantsInfo.Add(Combatant, FFightCombatantsInfo(Combatant->Profile->ProfileName, CombatantIndex));

			//TurnBasedCombatSubsystem->ProfileStartFight(Profile, this);
			//EXTRACTED FROM TurnBasedCombatSubsystem->ProfileStartFight

			if (AActor* ExplorationActor = TurnBasedCombatSubsystem->GetExplorationActor(Profile))
			{
				SavedPositions.Add(Profile, ExplorationActor->GetTransform());
				ExplorationActor->Destroy();
			}

			if (Profile->IsPlayerMainProfile())
			{
				//if (true || !Profile->Strategist)//We don't have the strategist already created
				//{

					APlayerController* PC = UBAMultiplayerSubsystem::GetPlayerControllerFromUser(this, Profile->BAUser);
					if (!PC)
					{
						UE_LOG(LogTurnBasedCombat, Error, TEXT("UTurnBasedCombatSubsystem::ProfileStartFight() PlayerController is null"));
						return;
					}

					if (APawn* Pawn = PC->GetPawn())
					{
						Pawn->Destroy();// it can't be a ITurnBased Strategist, we destroy it// TODO SEE IF THIS IS ACTUALLY EVER EXECUTED
					}

					if (!PlayerCombatPawnClass)
					{
						UE_LOG(LogTurnBasedCombat, Error, TEXT("UTurnBasedCombatSubsystem::ProfileStartFight() PlayerCombatPawnClass is null"));
						return;
					}
					//END: Checks

					//Spawn a new Strategist
					APawn* CombatPawn = World->SpawnActor<APawn>(PlayerCombatPawnClass, GetActorTransform());
					PC->Possess(CombatPawn);

					Profile->Strategist = CombatPawn;

					if (!Profile->Strategist)
					{
						UE_LOG(LogTurnBasedCombat, Error, TEXT("UTurnBasedCombatSubsystem::ProfileStartFight() PlayerCombatPawnClass is not a ITurnBasedStrategist"));
						return;
					}
					Profile->Strategist->Configure(Profile, Combatant);
				//}
			}

			if (Profile->Strategist)
			{
				Profile->Strategist->StartFight(this);

			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Profile doesn't have an strategist assigned! %s"), *Profile->GetName());
			}

			//END EXTRACTED FROM TurnBasedCombatSubsystem->ProfileStartFight

//#pragma region AddCombatantTurn
//			int32 const NewTurnIndex = Turns.Emplace();
//			FFightTurn& Turn = Turns[NewTurnIndex];
//			Turn.Combatant = Combatant;
//			Turn.Profile = Profile;
//			Turn.CombatantInfo = FFightCombatantsInfo(Combatant->Profile->ProfileName, CombatantIndex);
//#pragma endregion AddCombatantTurn
			//Turn.Profile->Strategist = TurnBasedCombatSubsystem->GetStrategist(Profile);
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

	if (CurrentTurn.Profile->Strategist)
	{
		CurrentTurn.Profile->Strategist->EndTurn();
	}


	TurnIndex = (TurnIndex + 1) % Turns.Num();
	FFightTurn& NewTurn = Turns[TurnIndex];

	if (NewTurn.Profile->Strategist)
	{
		NewTurn.Profile->Strategist->StartTurn(NewTurn.Profile, NewTurn.Combatant);
	}

	//if (GetWorld())
	//{
	//	FTimerHandle Handle;
	//	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AFight::TestDelayNotifyTurn, 5.f, false);
	//}
}

void AFight::NewRound()
{
	int CombatantIndex = 0;
	Turns.Empty();
	TurnIndex = 0;

	for (const TPair<UBAProfile*, ACombatant*>& pair : Combatants)
	{
		UFighterProfile* Profile = Cast<UFighterProfile>(pair.Key);
		ACombatant* Combatant = pair.Value;
		//Si esta muerto no se incluye en la lista
		//if(Combatant)
		int32 const NewTurnIndex = Turns.Emplace();
		FFightTurn& Turn = Turns[NewTurnIndex];
		Turn.Combatant = Combatant;
		Turn.Profile = Profile;
		Turn.CombatantInfo = FFightCombatantsInfo(Combatant->Profile->ProfileName, CombatantIndex, Combatant->ID);
		CombatantIndex++;
	}

	//Ahora siempre pone al jugador el primero y la IA va cambiando de orden de forma aleatoria
	const int32 NumShuffles = Turns.Num() - 1;
	for (int32 i = 1; i <= NumShuffles; ++i)
	{
		//if (Turns[i].CombatantInfo.CombatantName == "Malote")
		//{
		//	int32 SwapIdx = 0;//FMath::RandRange(i, NumShuffles);
		//	Turns[i].CombatantInfo.Pos = SwapIdx;
		//	Turns[SwapIdx].CombatantInfo.Pos = i;
		//	Turns.Swap(i, SwapIdx);
		//}
		//if (Turns[i].CombatantInfo.CombatantName == "Heisto")
		//{
		//	int32 SwapIdx = 1;//FMath::RandRange(i, NumShuffles);
		//	Turns[i].CombatantInfo.Pos = SwapIdx;
		//	Turns[SwapIdx].CombatantInfo.Pos = i;
		//	Turns.Swap(i, SwapIdx);
		//}
		int32 SwapIdx = FMath::RandRange(i, NumShuffles);
		Turns[i].CombatantInfo.Pos = SwapIdx;
		Turns[SwapIdx].CombatantInfo.Pos = i;
		Turns.Swap(i, SwapIdx);
	}
	//TODO Delete random order, the order depends on a position factor that the combatants will have.


	NetMulticast_OnNewRoundEvent(Turns);
	
}

void AFight::NotifyNextTurn()
{
	if (TurnIndex >= 0 && TurnIndex < Turns.Num() && Turns[TurnIndex].Combatant)
	{
		Turns[TurnIndex].Combatant->EndTurnUpdateValues();
	}

	TurnIndex++;
	if (Turns.Num() == 0 || TurnIndex < 0 || TurnIndex >= Turns.Num())
	{
		NewRound();
		if (Turns.Num() == 0)
		{
			UE_LOG(LogTurnBasedCombat, Log, TEXT("NotifyNextTurn Doesn't have a turn"));
			return;
		}
	}
	
	
	for(; TurnIndex  < Turns.Num(); TurnIndex++)
	{
		FFightTurn& NewTurn = Turns[TurnIndex];
		if (!NewTurn.Combatant->IsDead())
		{
			if (NewTurn.Combatant)
			{
				NewTurn.Combatant->StartTurnUpdateValues();
			}
			if (NewTurn.Profile->Strategist)
			{
				NewTurn.Profile->Strategist->StartTurn(NewTurn.Profile, NewTurn.Combatant);
			}
			break;
		}
	}
	if (TurnIndex >= Turns.Num())
	{
		NotifyNextTurn();
	}
	else 
	{
		NetMulticast_OnNewTurnEvent(TurnIndex);
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
		//TODO: find a way to replicate Teams!! it is nullptr in clients!!
		if (!Team)
		{
			continue;//TODO: all teams that are created in the server are nullptr in the clients!
		}

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

bool AFight::PerformAction(const UActionType* Action, ITurnBasedStrategist* Strategist, ACombatant* Sender, ACombatant* Receiver)
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

	//if (Strategist != TurnBasedCombatSubsystem->GetStrategist(Turns[TurnIndex].Profile))
	//{
	//	UE_LOG(LogTurnBasedCombat, Error, TEXT("The Strategists aren't the same"));
	//	return false;
	//}

	//END VALIDATIONS, we only need to do the Action validations themselves

#pragma endregion



	if (const UActionType_Trpg* TrpgAction = Cast<UActionType_Trpg>(Action))
	{
		FTrpgPerformActionRequest Request;
		Request.Action = TrpgAction;
		Request.Sender = Sender;
		Request.Receiver = Receiver;

		FTrpgPerformActionResult Result;

		if (Sender->GetBAProfile()->IsPlayerMainProfile())
		{
			if (!Sender->ValidateAction(TrpgAction, Request, Result))
			{
				OnCombatLog.Broadcast(Result.Log);
				return false;
			}
		}
		else //This is for the AI, it is necesary? We controll the AI
		{
			if (!TrpgAction->Validate(Request, Result))
			{
				OnCombatLog.Broadcast(Result.Log);
				return false;
			}
		}

		SetTurnState(ETurnState::PERFORMING_ACTION_STAGE);
		Sender->StartPerformingAction(TrpgAction, Receiver);

		//We save the latest action so we can repeat it
		//Turns[TurnIndex].LastActionPerformed = TrpgAction;

		float SequenceDuration = Arena->PerformActionSequence(Action, Sender, Receiver);


		if (SequenceDuration > 0.f)
		{
			FTimerDelegate TimerCallback;
			TimerCallback.BindWeakLambda(this, [this, Request]
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

	if (const UActionType_Trpg* Action = Cast<UActionType_Trpg>(Request.Action))
	{
		if (ACombatant* SenderCombatant = Cast <ACombatant>(Request.Sender))
		{
			Action->PerformAction(Request, Result);
			//PayAction(Action, SenderCombatant);
			SenderCombatant->PayActionWithoutValidation(Action);//The validation has been done in PerformAction.
		}
	}

	if (CheckCombatHasFinished())
	{
		for (UTeam* Team : Teams)
		{
			for (UFighterProfile* Profile : Team->Profiles)
			{
				if (UTurnBasedCombatSubsystem* TurnBasedCombatSubsystem = GetGameInstance()->GetSubsystem<UTurnBasedCombatSubsystem>())
				{
					//if (ITurnBasedStrategist* Strategist = TurnBasedCombatSubsystem->GetStrategist(Profile))
					//{
					//	if (Strategist)
					//	{
					//		Strategist->NotifyFightFinish(this);


					//	}
					//}
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
			FFightTurn->Combatant->EndPerformingAction();
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
	if (CurrentTurn.Profile->Strategist != Strategist)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("AFight::FinishTurn(): Strategist is not correct"));
		return;
	}

	CurrentTurn.Combatant->EndTurnUpdateValues();
	CurrentTurn.Profile->Strategist->EndTurn();
	NotifyNextTurn();
}

//void AFight::NetMulticast_OnCombatantDieOrRevive_Implementation(ACombatant* Combatant, bool IsDead)
//{
//	OnCombatantDieOrRevive.Broadcast(Combatant, IsDead);
//}

//void AFight::OnCombatantHealthChange(int32 OldHealth, int32 NewHealth, AActor* HealthOwner)
//{
//	ACombatant* Combatant = Cast<ACombatant>(HealthOwner);
//	if(Combatant == nullptr)
//		return;
//	//Ver si todos estan muertos y se acaba el combate, esto solo debe ejecutarlo el servidor.
//	if (NewHealth <= 0 || (OldHealth <= 0 && NewHealth > 0))
//	{
//		NetMulticast_OnCombatantDieOrRevive(Combatant, Combatant->IsDead());
//	}
//}

TArray<FFightTurn> AFight::GetTurns()
{
	return Turns;
	// TODO: insert return statement here
}

bool AFight::PayAction(UActionType_Trpg const* TrpgAction, ACombatant* Combatant)
{
	//bool const bHasEnoughActionPoints = Combatant->ActionPoints >= TrpgAction->GetActionPoints();
	//bool const bHastEnoughRemainingActions = Combatant->RemainingActions > 0;

	//if (bHasEnoughActionPoints && bHastEnoughRemainingActions)
	//{
	//	//Combatant->ActionPoints -= TrpgAction->GetActionPoints();
	//	
	//	Combatant->RemainingActions--;
	//	return true;
	//}
	//return false;

	return Combatant->TryPerformAction() && Combatant->TryConsumeActionPoints(TrpgAction->GetActionPoints());
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
				FTransform Transform = CurrentTransform;
				if (SavedPositions.Contains(Profile))
				{
					Transform = SavedPositions[Profile];
				}
				TurnBasedCombatSubsystem->StartExplorationMode(Profile, this, IsDead, Transform);
			}
			AActor* Pawn = Cast<AActor>(Combatant->Profile->Strategist.GetObject());
			if (Pawn)
				Pawn->Destroy();
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

void AFight::OnRep_TurnIndex()
{
	//FFightTurnInfo Info = FFightTurnInfo();
	if (TurnIndex < 0 || TurnIndex >= Turns.Num())
		return;
}

void AFight::NetMulticast_OnNewRoundEvent_Implementation(const TArray<FFightTurn>& NTurns)
{
	if (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer)
	{
		return;
	}
	OnNewRound.Broadcast(NTurns);

}

void AFight::NetMulticast_OnNewTurnEvent_Implementation(int Turn)
{
	OnNewTurnCombatant.Broadcast(Turns[Turn]);
}
