// Created by Bionic Ape. All Rights Reserved.


#include "Arena.h"
#include "Net/UnrealNetwork.h"
#include "Components/BillboardComponent.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Fight.h"
#include "Components/FighterComponent.h"
#include "MovieSceneSequencePlayer.h"
#include "Animation/PreviewAssetAttachComponent.h"
#include "Team.h"
#include "CombatantSkeletal.h"
#include "Arena/ArenaPerformer.h"
#include "DefaultLevelSequenceInstanceData.h"
#include "Arena/ArenaSequences.h"
#include "Arena/ArenaLevelSequenceActor.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/BAUserOwner.h"
#include "Interfaces/TurnBasedStrategist.h"


// Sets default values
AArena::AArena()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//SetReplicates(true);//I commented it because it says "Directly setting bReplicates is the correct procedure for pre-init actors."
	bReplicates = true;//Directly setting bReplicates is the correct procedure for pre-init actors.
	SetReplicateMovement(false);

	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("RootComponent"));

	OrbitCameraLocation = CreateDefaultSubobject<UBillboardComponent>(TEXT("OrbitCameraPawn"));
	OrbitCameraLocation->SetupAttachment(RootComponent);
	OrbitCameraLocation->SetRelativeLocation(FVector(0.f, 0.f, 150.f));

	TeamA_01 = CreateDefaultSubobject<UBillboardComponent>(TEXT("TeamA_01"));
	TeamA_01->SetupAttachment(RootComponent);

	TeamA_02 = CreateDefaultSubobject<UBillboardComponent>(TEXT("TeamA_02"));
	TeamA_02->SetupAttachment(RootComponent);

	TeamA_03 = CreateDefaultSubobject<UBillboardComponent>(TEXT("TeamA_03"));
	TeamA_03->SetupAttachment(RootComponent);

	TeamB_01 = CreateDefaultSubobject<UBillboardComponent>(TEXT("TeamB_01"));
	TeamB_01->SetupAttachment(RootComponent);

	TeamB_02 = CreateDefaultSubobject<UBillboardComponent>(TEXT("TeamB_02"));
	TeamB_02->SetupAttachment(RootComponent);

	TeamB_03 = CreateDefaultSubobject<UBillboardComponent>(TEXT("TeamB_03"));
	TeamB_03->SetupAttachment(RootComponent);
}

void AArena::Reset()
{
	Super::Reset();
	Fight = nullptr;
}

void AArena::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AArena, Fight);
	DOREPLIFETIME(AArena, ArenaPerformance);
}

void AArena::BeginPlay()
{
	Super::BeginPlay();

	if (ArenaSequences && GetWorld() /*&& GetLocalRole() == ROLE_Authority*/)//TODO: Check if we need this in the server
	{
		for (auto SequenceIt = ArenaSequences->Sequences.CreateIterator(); SequenceIt; ++SequenceIt)
		{
			const UActionType* Action = SequenceIt.Key();
			ULevelSequence* Sequence = SequenceIt.Value();

			AArenaLevelSequenceActor* SequenceActor = GetWorld()->SpawnActor<AArenaLevelSequenceActor>(AArenaLevelSequenceActor::StaticClass(), GetActorTransform());
			SequenceActor->SetSequence(Sequence);

			SequenceActor->bOverrideInstanceData = true;

			if (UDefaultLevelSequenceInstanceData* DefaultInstanceData = Cast<UDefaultLevelSequenceInstanceData>(SequenceActor->DefaultInstanceData))
			{
				DefaultInstanceData->TransformOriginActor = SequenceRootActor;
			}

			if (ULevelSequencePlayer* LevelSequencePlayer = SequenceActor->GetSequencePlayer())
			{
				LevelSequencePlayer->OnFinished.AddDynamic(this, &AArena::OnLevelSequenceFinished);
			}
			SequencesSpawned.Add(Action, SequenceActor);
		}
	}
}


void AArena::StartNewFight(AFight* NewFight)
{
	Fight = NewFight;
}


float AArena::PerformActionSequence(UActionType* Action, AActor* Sender, AActor* Receiver)
{
	ArenaPerformance.Action = Action;
	ArenaPerformance.Sender = Sender;
	ArenaPerformance.Receiver = Receiver;
	ArenaPerformance.RandomNumber = FMath::Rand();

	if (SequencesSpawned.Contains(ArenaPerformance.Action))
	{
		if (AArenaLevelSequenceActor* SequenceActor = SequencesSpawned.FindChecked(ArenaPerformance.Action))
		{
			if (ULevelSequencePlayer* LevelSequencePlayer = SequenceActor->GetSequencePlayer())
			{
				return LevelSequencePlayer->GetDuration().AsSeconds();
			}
		}
	}

	return 0.f;
}

void AArena::RemoveSelectedActors()
{
	for (TSoftObjectPtr<AActor> SelectionCircleActor : SelectionCircleArray)
	{
		if (SelectionCircleActor.IsValid())
		{
			SelectionCircleActor->Destroy();
		}
	}
	SelectionCircleArray.Empty();
}

void AArena::SetSelectedActor(AActor* SelectedActor)
{
	RemoveSelectedActors();

	if (SelectedActor && SelectionCircleClass)
	{
		FActorSpawnParameters Params;
		SelectionCircleArray.Add(GetWorld()->SpawnActor<AActor>(SelectionCircleClass, SelectedActor->GetActorTransform()));
	}
}


void AArena::OnRep_ArenaPerformance()
{
	if (SequencesSpawned.Contains(ArenaPerformance.Action))
	{
		if (AArenaLevelSequenceActor* SequenceActor = SequencesSpawned.FindChecked(ArenaPerformance.Action))
		{
			SequenceActor->ResetBindings();
			SequenceActor->AddBindingByTag(TEXT("Sender"), ArenaPerformance.Sender);
			SequenceActor->AddBindingByTag(TEXT("Receiver"), ArenaPerformance.Receiver);
			SequenceRootActor->SetActorTransform(ArenaPerformance.Receiver->GetActorTransform());

			if (ULevelSequencePlayer* LevelSequencePlayer = SequenceActor->GetSequencePlayer())
			{
				//Check if Local Player Controller is in this fight
				bool bDisableCameraCuts = true;//By default, we disable camera cuts, we only want to watch the cameras if the player controller is in the fight

				if (APawn* MyLocalPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
				{
					if (ITurnBasedStrategist* Strategist = Cast<ITurnBasedStrategist>(MyLocalPawn))
					{
						bDisableCameraCuts = (Fight != Strategist->GetFight());// we disable this camera cuts if our Local Player Controller's fight is not the same as this arena's fight
					}
				}
				LevelSequencePlayer->SetDisableCameraCuts(bDisableCameraCuts);
				LevelSequencePlayer->Play();
			}
		}
	}
}

void AArena::OnLevelSequenceFinished()
{
	if (ArenaPerformance.Sender && ArenaPerformance.Sender)
	{
		ArenaPerformance.Sender->SetActorHiddenInGame(false);
		ArenaPerformance.Receiver->SetActorHiddenInGame(false);
	}
}

const FTransform& AArena::GetPawnTransform(int32 TeamIndex, int32 WarriorIndex) const
{
	if (TeamIndex == 0)
	{
		switch (WarriorIndex)
		{
		case 0:
			return TeamA_01->GetComponentTransform();
		case 1:
			return TeamA_02->GetComponentTransform();
		case 2:
			return TeamA_03->GetComponentTransform();
		}
	}
	else {
		switch (WarriorIndex)
		{
		case 0:
			return TeamB_01->GetComponentTransform();
		case 1:
			return TeamB_02->GetComponentTransform();
		case 2:
			return TeamB_03->GetComponentTransform();
		}
	}
	return RootComponent->GetComponentTransform();
}

const FTransform& AArena::GetOrbitCameraTransform() const
{
	return OrbitCameraLocation->GetComponentTransform();
}