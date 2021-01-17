// Created by Bionic Ape. All Rights Reserved.

#include "CharacterSpawner.h"
#include "Components/SceneComponent.h"
#include "Components/BillboardComponent.h"

ACharacterSpawner::ACharacterSpawner() : Super()
{
	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
	RootComponent = RootSceneComp;

	BillBoardComp = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillBoardComp"));
	BillBoardComp->SetupAttachment(RootSceneComp);
}
