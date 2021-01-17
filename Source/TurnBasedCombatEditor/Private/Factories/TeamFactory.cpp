// Created by Bionic Ape. All Rights Reserved.

#include "Factories/TeamFactory.h"
#include "Team.h"

UTeamFactory::UTeamFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UTeam::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UTeamFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UTeam* NewAsset = NewObject<UTeam>(InParent, Class, Name, Flags);
	return NewAsset;
}