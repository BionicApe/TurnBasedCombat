// Created by Bionic Ape. All Rights Reserved.


#include "Components/FighterComponent.h"
#include "Team.h"
#include "Fight.h"
#include "PlayerCombatPawn.h"
#include "TurnBasedCombat.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "TurnBasedCombatLib.h"
#include "Arena.h"
#include "TurnBasedCombatSubsystem.h"

UFighterComponent::UFighterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFighterComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UFighterComponent::CreateFight(UFighterComponent* EnemyCombatComp)
{
	Server_CreateFight(EnemyCombatComp);
}

void UFighterComponent::Server_CreateFight_Implementation(UFighterComponent* EnemyFighterComp)
{
	if (EnemyFighterComp)
	{
		if (UWorld* World = GetWorld())
		{
			if (APawn* Pawn = GetPawnOwner())//TODO: Change it to GetPawnOwner 
			{
				if (APlayerController* PC = Cast<APlayerController>(Pawn->GetController()))
				{
					AFight* Fight = World->SpawnActor<AFight>(AFight::StaticClass(), FTransform::Identity);

					AArena* Arena = UTurnBasedCombatLib::FindArena(World, Pawn->GetActorLocation());
					if (Arena)
					{
						Fight->Arena = Arena;
						Arena->Fight = Fight;

						Fight->AddTeam(GetTeam());
						Fight->AddTeam(EnemyFighterComp->GetTeam());

						Fight->StartFight();

						//MyPawn
						//We Destroy Pawns
					}
				}
			}
		}
	}
	UE_LOG(LogTurnBasedCombat, Warning, TEXT("UFighterComponent::Server_CreateFight_Implementation: Couldn't APlayerCombatPawn"));
}

bool UFighterComponent::Server_CreateFight_Validate(UFighterComponent* EnemyPawnCombatComp)
{
	return true;
}

UTeam* UFighterComponent::GetTeam() const
{
	return nullptr;
}
