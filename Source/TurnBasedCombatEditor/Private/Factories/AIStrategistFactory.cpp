// Created by Bionic Ape. All Rights Reserved.

#include "Factories/AIStrategistFactory.h"
#include "AIStrategist.h"

UAIStrategistFactory::UAIStrategistFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UAIStrategist::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UAIStrategistFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UAIStrategist* NewAsset = NewObject<UAIStrategist>(InParent, Class, Name, Flags);
	return NewAsset;
}


