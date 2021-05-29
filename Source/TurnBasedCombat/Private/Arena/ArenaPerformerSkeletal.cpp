// Created by Bionic Ape. All Rights Reserved.


#include "Arena/ArenaPerformerSkeletal.h"
#include "Actor.h"

void AArenaPerformerSkeletal::Setup(AActor* Combatant)
{
	SetActorHiddenInGame(false);
}

void AArenaPerformerSkeletal::Teardown()
{
	SetActorHiddenInGame(true);
}
