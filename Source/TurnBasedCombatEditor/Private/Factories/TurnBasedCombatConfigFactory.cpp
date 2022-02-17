// Created by Bionic Ape. All Rights Reserved.

#include "Factories/TurnBasedCombatConfigFactory.h"
#include "Config/TurnBasedCombatConfig.h"

UTurnBasedCombatConfigFactory::UTurnBasedCombatConfigFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UTurnBasedCombatConfig::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UTurnBasedCombatConfigFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UTurnBasedCombatConfig* NewAsset = NewObject<UTurnBasedCombatConfig>(InParent, Class, Name, Flags);
	return NewAsset;
}


