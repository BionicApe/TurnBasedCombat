// Created by Bionic Ape. All Rights Reserved.


#include "CombatTurnManager.h"

// Sets default values
ACombatTurnManager::ACombatTurnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACombatTurnManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACombatTurnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatTurnManager::AddActionToHistory(FTurnActionHistoryItem Item)
{
	ActionsHistory.Add(Item);
}

TArray<FTurnActionHistoryItem> ACombatTurnManager::GetActionsHistory()
{
	return ActionsHistory;
}

FTurnActionHistoryItem const* ACombatTurnManager::GetActionHistory(int Index)
{
	if (ActionsHistory.Num() >= Index)
		return nullptr;
	return &ActionsHistory[Index];
}

