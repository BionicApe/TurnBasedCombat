// Created by Bionic Ape. All Rights Reserved.


#include "FighterProfile.h"
#include "BARPGPersona.h"
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
	//TODO: Better way of leveling up
	{
		Attributes.XPLevel = Attributes.Xp / 100;//This should be replaced
	}
	OnAttributesChanged.Broadcast();
	return Attributes.Xp;
}

int32 UFighterProfile::AddAttributePoints(int32 PointsToAdd)
{
	Attributes.AttributePoints += PointsToAdd;
	OnAttributesChanged.Broadcast();
	return Attributes.AttributePoints;
}

bool UFighterProfile::AddAttributePoint(FString AttributeName)
{
 	if (Attributes.AttributePoints > 0)
	{
		if (AttributeName == "Strength")
		{
			Attributes.AttributePoints--;
			Attributes.Strength++;
			OnAttributesChanged.Broadcast();
			return true;
		}
		else if (AttributeName == "Dexterity")
		{
			Attributes.AttributePoints--;
			Attributes.Dexterity++;
			OnAttributesChanged.Broadcast();
			return true;
		}
		else if (AttributeName == "Agility")
		{
			Attributes.AttributePoints--;
			Attributes.Agility++;
			OnAttributesChanged.Broadcast();
			return true;
		}
		else if (AttributeName == "Vitality")
		{
			Attributes.AttributePoints--;
			Attributes.Vitality++;
			OnAttributesChanged.Broadcast();
			return true;
		}
		else if (AttributeName == "Intelligence")
		{
			Attributes.AttributePoints--;
			Attributes.Intelligence++;
			OnAttributesChanged.Broadcast();
			return true;
		}
		else if (AttributeName == "Charisma")
		{
			Attributes.AttributePoints--;
			Attributes.Charisma++;
			OnAttributesChanged.Broadcast();
			return true;
		}
	}
	return false;
}

void UFighterProfile::OnRep_Attributes()
{
	OnAttributesChanged.Broadcast();
}

bool UFighterProfile::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	WroteSomething |= Channel->ReplicateSubobject(Inventory, *Bunch, *RepFlags);
	WroteSomething |= Channel->ReplicateSubobject(Persona, *Bunch, *RepFlags);
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