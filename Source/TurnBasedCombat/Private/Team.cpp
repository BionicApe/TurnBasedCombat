// Created by Bionic Ape. All Rights Reserved.


#include "Team.h"
#include "Components/FighterComponent.h"
#include "UObject/CoreNet.h"
#include "Net/UnrealNetwork.h"

void UTeam::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTeam, Profiles);
}