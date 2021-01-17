// Created by Bionic Ape. All Rights Reserved.


#include "CombatantSkeletal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "UI/HealthWidgetComponent.h"
#include "Interfaces/UseInventoryItem.h"
#include "Animation/CombatantAnimInstance.h"
#include "Inventory/InventoryItem.h"

ACombatantSkeletal::ACombatantSkeletal() : Super()
{
	SkeletalComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalComp"));
	//SkeletalComp->AlwaysLoadOnClient = true;
	//SkeletalComp->AlwaysLoadOnServer = false;
	//SkeletalComp->bOwnerNoSee = false;
	//SkeletalComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	//SkeletalComp->bCastDynamicShadow = true;
	//SkeletalComp->bAffectDynamicIndirectLighting = true;
	//SkeletalComp->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	//SkeletalComp->SetCollisionProfileName(TEXT("CharacterMesh"));
	//SkeletalComp->SetGenerateOverlapEvents(false);
	//SkeletalComp->SetCanEverAffectNavigation(false);
	SetRootComponent(SkeletalComp);

	WidgetComp = CreateDefaultSubobject<UHealthWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WidgetComp->SetupAttachment(GetRootComponent());
	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 200.f));

}

void ACombatantSkeletal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACombatantSkeletal, AnimIndex);
}

void ACombatantSkeletal::OnNewItemSelected(UInventoryItem* InventoryItem)
{
	Super::OnNewItemSelected(InventoryItem);
	AnimIndex = InventoryItem->FightingStyle;
}

void ACombatantSkeletal::OnRep_AnimIndex()
{
	if (UCombatantAnimInstance* AnimInstance = Cast<UCombatantAnimInstance>(SkeletalComp->GetAnimInstance()))
	{
		AnimInstance->AnimIndex = AnimIndex;
	}
}

void ACombatantSkeletal::OnDie(FActorKilled ActorKilledProperties)
{
	Super::OnDie(ActorKilledProperties);

	//SkeletalComp->SetCollisionProfileName(TEXT("Ragdoll"));
	//SkeletalComp->SetSimulatePhysics(true);
}

