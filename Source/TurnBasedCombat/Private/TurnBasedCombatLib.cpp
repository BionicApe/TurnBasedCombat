// Created by Bionic Ape. All Rights Reserved.


#include "TurnBasedCombatLib.h"
#include "Arena.h"
#include "Kismet/GameplayStatics.h"
#include "TurnBasedCombat.h"
#include "FighterProfile.h"
#include "Components/TrpgControlComponent.h"
#include "Interfaces/TrpgControlOwner.h"

AArena* UTurnBasedCombatLib::FindArena(const UObject* WorldContextObject, FVector const& Location)
{
	TArray<AActor*> ArenasFound;
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, AArena::StaticClass(), ArenasFound);

	if (ArenasFound.Num() == 0)
	{
		UE_LOG(LogTurnBasedCombat, Error, TEXT("UTurnBasedCombatLib::FindArena(): Couldn't Find an Arena"));
		return nullptr;
	}


	AArena* ClosestArena = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();

	for (int32 i = 0; i < ArenasFound.Num(); i++)
	{
		AArena* ArenaToCheck = Cast<AArena>(ArenasFound[i]);//It's safe to cast and not check

		if (ArenaToCheck->IsAvailableForANewFight())//If is not available we skip it
		{
			if (!ClosestArena)//We might have not found an available arena yet
			{
				ClosestDistance = (ArenaToCheck->GetActorLocation() - Location).Size();
				ClosestArena = ArenaToCheck;
			}
			else
			{
				float const NewDistance = (ArenaToCheck->GetActorLocation() - Location).Size();
				if (NewDistance < ClosestDistance)
				{
					ClosestArena = ArenaToCheck;
					ClosestDistance = NewDistance;
				}
			}
		}
	}

	return ClosestArena;
}

UTrpgControlComponent* UTurnBasedCombatLib::GetTrpgControlComp(APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		return nullptr;
	}

	if (ITrpgControlOwner* TrpgControlOwner = Cast<ITrpgControlOwner>(PlayerController))
	{
		return TrpgControlOwner->GetTrpgControlComp();//We assume It's been implemented correctly
	}
	else
	{
		//Developer didn't implement ITrpgControlOwner but it might have UTrpgControlComponent as a regular component
		return Cast<UTrpgControlComponent>(PlayerController->GetComponentByClass(UTrpgControlComponent::StaticClass()));
	}
}

UFighterProfile* UTurnBasedCombatLib::GetMainFighterProfile(APlayerController* PlayerController)
{
	UTrpgControlComponent* TrpgContComp = UTurnBasedCombatLib::GetTrpgControlComp(PlayerController);
	return TrpgContComp ? TrpgContComp->GetMainFighterProfile() : nullptr;
}
