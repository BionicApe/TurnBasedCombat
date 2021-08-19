// Created by Bionic Ape. All Rights Reserved.


#include "FighterProfile.h"
#include "Inventory/Inventory.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


void UFighterProfile::SetActionPoints(int32 NewActionPoints)
{
	ActionPoints = NewActionPoints;
	OnProfileActionPointsChanged.Broadcast();
}

void UFighterProfile::OnRep_ActionPoints()
{
	OnProfileActionPointsChanged.Broadcast();
}

void UFighterProfile::SetActionsPerTurn(int32 NewActionsPerTurn)
{
	ActionsPerTurn = NewActionsPerTurn;
	OnActionsPerTurnChanged.Broadcast();
}

void UFighterProfile::OnRep_ActionsPerTurn()
{
	OnActionsPerTurnChanged.Broadcast();
}

void UFighterProfile::SetAttributes(FFighterAttributes NewAttributes)
{
	Attributes = NewAttributes;
	OnAttributesChanged.Broadcast();
}

int32 UFighterProfile::AddXp(int32 XpToAdd)
{
	Attributes.Xp += XpToAdd;
	OnAttributesChanged.Broadcast();
	return Attributes.Xp;
}

int32 UFighterProfile::AddAttributePoints(int32 PointsToAdd)
{
	Attributes.AttributePoints += PointsToAdd;
	OnAttributesChanged.Broadcast();
	return Attributes.AttributePoints;
}

void UFighterProfile::OnRep_Attributes()
{
	OnAttributesChanged.Broadcast();
}

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
	DOREPLIFETIME(UFighterProfile, Attributes);
}