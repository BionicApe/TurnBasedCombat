// Created by Bionic Ape. All Rights Reserved.


#include "Arena/ArenaPerformerSkeletal.h"

void AArenaPerformerSkeletal::Setup(AActor* Combatant)
{
	SetActorHiddenInGame(false);
}

void AArenaPerformerSkeletal::Teardown()
{
	SetActorHiddenInGame(true);
}
