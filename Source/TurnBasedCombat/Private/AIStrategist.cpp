// Created by Bionic Ape. All Rights Reserved.


#include "AIStrategist.h"
#include "Team.h"
#include "Combatant.h"
#include "Fight.h"
#include "ActionType.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UAIStrategist::StartFight(AFight* NewFight)
{

}

void UAIStrategist::StartTurn(UBAProfile* MyProfile, ACombatant* MyCombatant)
{
	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([this, MyProfile, MyCombatant]
		{
			if (MyProfile && MyCombatant)
			{
				if (AFight* Fight = MyCombatant->Fight)
				{
					UTeam* EnemyTeam = nullptr;

					for (UTeam* TeamToCheck : Fight->Teams)
					{
						if (!TeamToCheck->Profiles.Contains(MyProfile))
						{
							EnemyTeam = TeamToCheck;
							break;
						}
					}
					if (EnemyTeam && EnemyTeam->Profiles.Num() > 0)
					{

						int32 EnemyMaxIndex = EnemyTeam->Profiles.Num() - 1;
						int32 EnemyRandomIndex = FMath::RandRange(0, EnemyMaxIndex);
						if (UBAProfile* RandomEnemyProfile = EnemyTeam->Profiles[EnemyRandomIndex])
						{
							if (ACombatant* EnemyCombatant = Fight->Combatants[RandomEnemyProfile])
							{
								Fight->PerformAction(DefaultAction, this, MyCombatant, EnemyCombatant);
								return;
							}
						}
					}
				}
			}
		});

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, TimerCallback, 4.f, false);
	FTimerHandle Handle2;
	GetWorld()->GetTimerManager().SetTimer(Handle2, TimerCallback, 10.f, false);

}

void UAIStrategist::EndTurn()
{

}

void UAIStrategist::NotifyFightFinish(AFight* FinishedFight)
{

}