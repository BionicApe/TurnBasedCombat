// Created by Bionic Ape. All Rights Reserved.


#include "Arena/ArenaLevelSequenceActor.h"

AArenaLevelSequenceActor::AArenaLevelSequenceActor(const FObjectInitializer& Init) : Super(Init)
{
	bReplicates = false;
	SetReplicates(false);
	
	bReplicatePlayback = false;

	PlaybackSettings.bAutoPlay = false;
	PlaybackSettings.bHideHud = true;
	PlaybackSettings.bRestoreState = true;
}