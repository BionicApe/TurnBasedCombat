// Created by Bionic Ape. All Rights Reserved.


#include "UI/CombatUITurnInfo.h"

UCombatUITurnInfo* UCombatUITurnInfo::NEW()
{
	UCombatUITurnInfo* obj = NewObject<UCombatUITurnInfo>();
	return obj;
}

UCombatUITurnInfo* UCombatUITurnInfo::NEW(FString _CombatantName, int _Pos, FString _ID)
{
	UCombatUITurnInfo* obj = NEW();
	obj->CombatantName = _CombatantName;
	obj->Pos = _Pos;
	obj->ID = _ID;
	return obj;
}
