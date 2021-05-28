// Created by Bionic Ape. All Rights Reserved.


#include "Arena/ArenaLevelSequenceActor.h"

AArenaLevelSequenceActor::AArenaLevelSequenceActor(const FObjectInitializer& Init) : Super(Init)
{
	bReplicates = false;//Directly setting bReplicates is the correct procedure for pre-init actors.
	
	bReplicatePlayback = false;

	PlaybackSettings.bAutoPlay = false;
	PlaybackSettings.bHideHud = true;
	PlaybackSettings.bRestoreState = true;
}