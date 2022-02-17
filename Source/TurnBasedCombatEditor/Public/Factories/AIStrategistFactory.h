// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "AIStrategistFactory.generated.h"

/**
*
*/
UCLASS()
class TURNBASEDCOMBATEDITOR_API UAIStrategistFactory : public UFactory
{
	GENERATED_BODY()


	UAIStrategistFactory(const FObjectInitializer& ObjectInitializer);

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override { return true; }
	// End of UFactory interface

};
