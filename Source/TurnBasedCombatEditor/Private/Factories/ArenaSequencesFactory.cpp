// Created by Bionic Ape. All Rights Reserved.

#include "Factories/ArenaSequencesFactory.h"
#include "Arena/ArenaSequences.h"

UArenaSequencesFactory::UArenaSequencesFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UArenaSequences::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UArenaSequencesFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UArenaSequences* NewAsset = NewObject<UArenaSequences>(InParent, Class, Name, Flags);
	return NewAsset;
}