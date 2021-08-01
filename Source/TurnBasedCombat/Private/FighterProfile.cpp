// Created by Bionic Ape. All Rights Reserved.


#include "FighterProfile.h"
#include "Inventory/Inventory.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


bool UFighterProfile::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	WroteSomething |= Channel->ReplicateSubobject(Inventory, *Bunch, *RepFlags);
	if (Inventory)
	{
		WroteSomething |= Inventory->ReplicateSubobjects(Channel, Bunch, RepFlags);
	}
	return WroteSomething;
}

void UFighterProfile::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFighterProfile, Inventory);
}
