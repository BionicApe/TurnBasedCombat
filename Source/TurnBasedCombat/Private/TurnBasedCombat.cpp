// Created by Bionic Ape. All Rights Reserved.

#include "TurnBasedCombat.h"

#define LOCTEXT_NAMESPACE "FTurnBasedCombatModule"

DEFINE_LOG_CATEGORY(LogTurnBasedCombat);

void FTurnBasedCombatModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FTurnBasedCombatModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTurnBasedCombatModule, TurnBasedCombat)