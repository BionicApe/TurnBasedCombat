// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Combatant.h"
#include "UObject/CoreNet.h"
#include "DamageSystemTypes.h"
#include "CombatantSkeletal.generated.h"

class UHealthWidgetComponent;
class UInventoryItem;
class USkeletalMeshComponent;
class UInventoryItem;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API ACombatantSkeletal : public ACombatant
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = TurnBasedCombat, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalComp;

	UPROPERTY(Category = TurnBasedCombat, EditAnywhere, ReplicatedUsing="OnRep_AnimIndex")
	uint8 AnimIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UHealthWidgetComponent* WidgetComp;

public:

	ACombatantSkeletal();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnNewItemSelected(UInventoryItem* InventoryItem) override;

	UFUNCTION()
	void OnRep_AnimIndex();

	virtual void OnDie(FActorKilled ActorKilledProperties) override;
};
