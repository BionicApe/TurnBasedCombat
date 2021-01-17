// Created by Bionic Ape. All Rights Reserved.


#include "Components/TrpgControllerComponent.h"
#include "GameFramework/Controller.h"
#include "Interfaces/TurnBasedStrategist.h"


#pragma region ToDelete
UTrpgControllerComponent::UTrpgControllerComponent() :Super()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}
#pragma endregion

#pragma region ToDelete
//TODO: Delete Tick when it's possible
void UTrpgControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (AController* Controller = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = Controller->GetPawn())
		{
			OnNewPawn(Pawn);
		}
	}
}
#pragma endregion

void UTrpgControllerComponent::OnRegister()
{
	Super::OnRegister();


	//TODO: This for some reason doesn't work, I posted a question on ue4 Answerhub https://answers.unrealengine.com/questions/997596/view.html

	if (AController* Controller = Cast<AController>(GetOwner()))
	{
		Controller->GetOnNewPawnNotifier().AddUObject(this, &UTrpgControllerComponent::OnNewPawn);

		if (APawn* Pawn = Controller->GetPawn())//Just in case controller already has a Pawn I think this would never happen, just in case
		{
			OnNewPawn(Pawn);
		}
	}
}

void UTrpgControllerComponent::SetIsInCombat(bool bNewValue)
{
	if (bNewValue != bIsInCombat)
	{
		bIsInCombat = bNewValue;

		if (bIsInCombat)
		{
			OnStartCombatMode.Broadcast();
		}
		else
		{
			OnFinishCombatMode.Broadcast();
		}
	}
}

void UTrpgControllerComponent::OnNewPawn(APawn* NewPawn)
{
	if (NewPawn)
	{
		bool const bIsAStrategist = NewPawn->Implements<UTurnBasedStrategist>();
		SetIsInCombat(bIsAStrategist);
	}
	else
	{
		SetIsInCombat(false);
	}
}
