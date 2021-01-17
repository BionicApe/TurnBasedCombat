// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TrpgControllerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombatControllerStateChanged);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TURNBASEDCOMBAT_API UTrpgControllerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:


	bool bIsInCombat = false;

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnCombatControllerStateChanged OnStartCombatMode;
	
	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnCombatControllerStateChanged OnFinishCombatMode;

public:
	UTrpgControllerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsInCombat() { return bIsInCombat; }

	void SetIsInCombat(bool bNewValue);

	virtual void OnRegister() override;

	UFUNCTION()
	virtual void OnNewPawn(APawn* NewPawn);
};
