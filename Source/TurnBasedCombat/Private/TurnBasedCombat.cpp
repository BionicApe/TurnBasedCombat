// Created by Bionic Ape. All Rights Reserved.

#include "TurnBasedCombat.h"
#include "TurnBasedCombatLog.h"


void FTurnBasedCombatModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FTurnBasedCombatModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

IMPLEMENT_MODULE(FTurnBasedCombatModule, TurnBasedCombat)