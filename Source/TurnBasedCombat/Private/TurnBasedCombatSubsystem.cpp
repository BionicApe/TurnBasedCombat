// Created by Bionic Ape. All Rights Reserved.


#include "TurnBasedCombatSubsystem.h"
#include "TurnBasedCombat.h"
#include "UObject/Object.h"
#include "Team.h"
#include "Interfaces/TurnBasedStrategist.h"
#include "AIStrategist.h"
#include "BAProfile.h"
#include "Fight.h"
#include "BAMultiplayerSubsystem.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "CharacterSpawner.h"
#include "EngineUtils.h"
#include "Interfaces/ProfileAsignable.h"
#include "GameFramework/PlayerController.h"
#include "FighterProfile.h"
#include "Config/TurnBasedCombatConfig.h"
#include "UObject/SoftObjectPtr.h"




UTurnBasedCombatSubsystem::UTurnBasedCombatSubsystem() :Super()
{

}

void UTurnBasedCombatSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogTurnBasedCombat, Log, TEXT("UTurnBasedCombatSubsystem Initialize"));
	Super::Initialize(Collection);

	if (!ConfigProxy.IsNull())
	{
		Config = ConfigProxy.LoadSynchronous();
	}

	//if (!AIStrategistClass)
	//{
	//	UE_LOG(LogTurnBasedCombat, Error, TEXT("AIStrategistClass is null"));
	//	AIStrategistClass = UAIStrategist::StaticClass();//Default Strategist Class
	//}

	//AIStrategistObj = NewObject<UObject>(this, AIStrategistClass, TEXT("AIStrategist"));
	//AIStrategist = Cast<ITurnBasedStrategist>(AIStrategistObj);

}

//ITurnBasedStrategist* UTurnBasedCombatSubsystem::GetStrategist(UFighterProfile* Profile) const
//{
//	if (Profile && StrategistsAssigned.Contains(Profile))
//	{
//		return static_cast<ITurnBasedStrategist*>(StrategistsAssigned[Profile].GetInterface());
//	}
//	return nullptr;
//}

AActor* UTurnBasedCombatSubsystem::GetExplorationActor(UBAProfile* Profile) const
{
	if (Profile && ExplorationActors.Contains(Profile))
	{
		return ExplorationActors[Profile];
	}
	return nullptr;
}

//void UTurnBasedCombatSubsystem::AddStrategist(UBAProfile* Profile, ITurnBasedStrategist* Strategist)
//{
//	//StrategistsAssigned.Add(Profile, Strategist);
//	TScriptInterface<ITurnBasedStrategist>& Interface = StrategistsAssigned.Emplace(Profile);
//	Interface.SetInterface(Strategist);
//	Interface.SetObject(Cast<UObject>(Strategist));
//}


//void UTurnBasedCombatSubsystem::RemoveStrategist(ITurnBasedStrategist* Strategist)
//{
//	for (auto It = StrategistsAssigned.CreateConstIterator(); It; ++It)
//	{
//		auto LoopStrategist = It.Value();
//		if (Strategist == LoopStrategist.GetInterface())
//		{
//			StrategistsAssigned.Remove(It.Key());
//		}
//	}
//}

void UTurnBasedCombatSubsystem::ProfileStartFight(UFighterProfile* Profile, AFight* Fight)
{
	//TODO: Remove default Strategist and create one when we are actually retrieving the profile

	if (AActor* ExplorationActor = GetExplorationActor(Profile))
	{
		ExplorationActor->Destroy();
	}

	//ITurnBasedStrategist* Strategist = GetStrategist(Profile);

	//if (Profile->IsPlayerMainProfile())
	//{
	//	if (!Strategist)//We don't have the strategist already created
	//	{
	//		//Checks
	//		UWorld* World = Fight->GetWorld();
	//		if (!World)
	//		{
	//			UE_LOG(LogTurnBasedCombat, Error, TEXT("UTurnBasedCombatSubsystem::ProfileStartFight() World is null"));
	//			return;
	//		}

	//		APlayerController* PC = UBAMultiplayerSubsystem::GetPlayerControllerFromUser(this, Profile->BAUser);
	//		if (!PC)
	//		{
	//			UE_LOG(LogTurnBasedCombat, Error, TEXT("UTurnBasedCombatSubsystem::ProfileStartFight() PlayerController is null"));
	//			return;
	//		}

	//		if (APawn* Pawn = PC->GetPawn())
	//		{
	//			Pawn->Destroy();// it can't be a ITurnBased Strategist, we destroy it// TODO SEE IF THIS IS ACTUALLY EVER EXECUTED
	//		}

	//		if (!Config->PlayerCombatPawnClass)
	//		{
	//			UE_LOG(LogTurnBasedCombat, Error, TEXT("UTurnBasedCombatSubsystem::ProfileStartFight() PlayerCombatPawnClass is null"));
	//			return;
	//		}
	//		//END: Checks

	//		//Spawn a new Strategist
	//		APawn* CombatPawn = World->SpawnActor<APawn>(Profile->PlayerCombatPawnClass, Fight->GetActorTransform());
	//		PC->Possess(CombatPawn);

	//		Strategist = Cast<ITurnBasedStrategist>(CombatPawn);
	//		Profile->Strategist = Strategist;
	//		if (!Strategist)
	//		{
	//			UE_LOG(LogTurnBasedCombat, Error, TEXT("UTurnBasedCombatSubsystem::ProfileStartFight() PlayerCombatPawnClass is not a ITurnBasedStrategist"));
	//			return;
	//		}
	//		AddStrategist(Profile, Strategist);
	//	}

	//}
	//else //it's not a profile of a User (player)
	//{
	//	if (!Strategist)
	//	{
	//		//We add the default strategist
	//		Strategist = AIStrategist;
	//		AddStrategist(Profile, AIStrategist);
	//	}
	//}
	//Strategist->StartFight(Fight);
}

void UTurnBasedCombatSubsystem::StartExplorationMode(UBAProfile* Profile, UObject* MyContext, bool const bIsDead, FTransform const CurrentTransform)
{
	//Spawn a new Strategist
	UWorld* World = MyContext->GetWorld();

	if (!World)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("UTurnBasedCombatSubsystem::StartExplorationMode() World is null"));
		return;
	}

	if (!Config->ExplorationPawnClass)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("UTurnBasedCombatSubsystem::StartExplorationMode() No ExplorationPawnClass Assigned"));
		return;
	}

	ACharacterSpawner* SpawnerActor = FindSpawnerActor(World, Profile);
	

	if (SpawnerActor)
	{
		APawn* ExplorationPawn = World->SpawnActor<APawn>(Config->ExplorationPawnClass, SpawnerActor->GetActorTransform());
					
		if (Profile->IsPlayerMainProfile())
		{
			UBAMultiplayerSubsystem* MultiplayerSubsystem = GetGameInstance()->GetSubsystem<UBAMultiplayerSubsystem>();
			//Find my Player Controller
			APlayerController* PC = UBAMultiplayerSubsystem::GetPlayerControllerFromUser(this, Profile->BAUser);
			if (PC)
			{
				PC->Possess(ExplorationPawn);
			}
		}
		else
		{
			//Spawn a AI controller
			AAIController* AIController = World->SpawnActor<AAIController>(Config->NpcAiControllerClass, ExplorationPawn->GetActorTransform());
			AIController->Possess(ExplorationPawn);
		}

		if (IProfileAsignable* ProfileAsignable = Cast<IProfileAsignable>(ExplorationPawn))
		{
			ProfileAsignable->SetBAProfile(Profile);
		}
	}


	//ITurnBasedStrategist* Strategist = GetStrategist(Profile);
	//RemoveStrategist(Strategist);
	//if (Strategist)
	//{
	//	if (AActor* StrategistActor = Cast<AActor>(Strategist))
	//	{
	//		StrategistActor->Destroy();
	//	}
	//}
}

ACharacterSpawner* UTurnBasedCombatSubsystem::FindSpawnerActor(UWorld* World, UBAProfile* Profile)
{
	for (TActorIterator<ACharacterSpawner> It(World); It; ++It)
	{
		ACharacterSpawner* CharacterSpawner = *It;
		if (CharacterSpawner->ProfileId == Profile->Id)
		{
			return CharacterSpawner;
		}
	}
	return nullptr;
}
