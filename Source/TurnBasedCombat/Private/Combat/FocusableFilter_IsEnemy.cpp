// Created by Bionic Ape. All Rights Reserved.


#include "Combat/FocusableFilter_IsEnemy.h"
#include "GameFramework/Actor.h"
//#include "Components/TrpgPawnComponent.h"

bool UFocusableFilter_IsEnemy::IsAllowed(AActor* FocusableActor, UFocusableComponent* FocusableComponent)
{
	//if (UTrpgPawnComponent const* const FocusedCombatComponent = Cast<UTrpgPawnComponent>(FocusableActor->GetComponentByClass(UTrpgPawnComponent::StaticClass())))
	//{
	//	if (FocusedCombatComponent->Warrior)
	//	{
	//		return MyWarrior->IsEnemy(FocusedCombatComponent->Warrior);
	//	}
	//}
	return false;
}
