// Created by Bionic Ape. All Rights Reserved.

#include "Combatant.h"
#include "Components/HealthComponent.h"
#include "BAProfile.h"
#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/HealthComponent.h"
#include "Components/FocusableComponent.h"
#include "FighterProfile.h"

// Sets default values
ACombatant::ACombatant()
{
	PrimaryActorTick.bCanEverTick = false;
	//SetReplicates(true);//Directly setting bReplicates is the correct procedure for pre-init actors
	bReplicates = true;
	SetReplicatingMovement(false);

	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComp"));
	InventoryComp->SetIsReplicated(true);
	InventoryComp->OnNewItemSelected.AddDynamic(this, &ACombatant::OnNewItemSelected);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComp->OnDieEvent.AddDynamic(this, &ACombatant::OnDie);
	HealthComp->OnTakeDamageEvent.AddDynamic(this, &ACombatant::OnTakeDamage);
	HealthComp->SetIsReplicated(true);

	FocusableComp = CreateDefaultSubobject<UFocusableComponent>(TEXT("FocusableComp"));
}

void ACombatant::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACombatant, Profile);
	DOREPLIFETIME(ACombatant, Fight);
	DOREPLIFETIME(ACombatant, ActionPoints);
	DOREPLIFETIME(ACombatant, RemainingActions);
	DOREPLIFETIME(ACombatant, bIsDead);
}

float ACombatant::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float ReturnValue = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HealthComp->TakeDamage(DamageAmount,DamageEvent,EventInstigator,DamageCauser);
	return ReturnValue;
}

void ACombatant::SetBAProfile(UBAProfile* NewProfile)
{
	Profile = Cast<UFighterProfile>(NewProfile);
	
	if (Profile)
	{
		InventoryComp->Inventory = Profile->Inventory;
	}
}

UBAProfile* ACombatant::GetBAProfile() const
{
	return Profile;
}

void ACombatant::OnNewItemSelected(UInventoryItem* InventoryItem)
{
	
}

void ACombatant::StartTurnUpdateValues()
{
	ActionPoints = Profile->ActionPoints;
	RemainingActions = Profile->AmountOfActionsPerTurn;
}

void ACombatant::EndTurnUpdateValues()
{	
	
}

void ACombatant::OnDie(FActorKilled ActorKilledProperties)
{
	bIsDead = true;
}

void ACombatant::OnTakeDamage(FTakeDamageProperties const TakeDamageProperties)
{

}

bool ACombatant::IsDead()
{
	return !HealthComp->IsAlive();
}

void ACombatant::OnRep_IsDead()
{
	if (bIsDead)
	{
		OnCombatantDie.Broadcast();
	}
}

