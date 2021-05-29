// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/CoreNet.h"
#include "Interfaces/ProfileAsignable.h"
#include "DamageSystemTypes.h"
#include "Engine/EngineTypes.h"
#include "Combatant.generated.h"

class UInventoryComponent;
class UBAProfile;
class UHealthComponent;
class UFocusableComponent;
class UInventoryItem;
class UFighterProfile;
class AFight;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombatantDIeDelegate);

UCLASS()
class TURNBASEDCOMBAT_API ACombatant : public AActor, public IProfileAsignable
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = TurnBasedCombat, EditAnywhere, Replicated)
	AFight* Fight;

	UPROPERTY(Category = TurnBasedCombat, EditAnywhere, Replicated)
	UFighterProfile* Profile;

	UPROPERTY(Category = TurnBasedCombat, EditAnywhere, Replicated)
	int32 ActionPoints;

	UPROPERTY(Category = TurnBasedCombat, EditAnywhere, Replicated)
	int32 RemainingActions = 2;

	UPROPERTY(BlueprintReadWrite, Category = TrpgCombat)
	int32 RemainingSeconds = 60;

	UPROPERTY(Category = TurnBasedCombat, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComp;

	UPROPERTY(Category = TurnBasedCombat, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComp;

	UPROPERTY(Category = TurnBasedCombat, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UFocusableComponent* FocusableComp;

	UPROPERTY(BlueprintAssignable)
	FOnCombatantDIeDelegate OnCombatantDie;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing="OnRep_IsDead")
	bool bIsDead = false;

public:

	ACombatant();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor * DamageCauser) override;

	virtual void SetBAProfile(UBAProfile* Profile) override;
	virtual UBAProfile* GetBAProfile() const override;

	UFUNCTION()
	virtual void OnNewItemSelected(UInventoryItem* InventoryItem);

	virtual void StartTurnUpdateValues();

	virtual void EndTurnUpdateValues();

	UFUNCTION()
	virtual void OnDie(FActorKilled ActorKilledProperties);
	
	UFUNCTION()
	virtual void OnTakeDamage(FTakeDamageProperties const TakeDamageProperties);

	virtual bool IsDead();

	UFUNCTION()
	virtual void OnRep_IsDead();
};
