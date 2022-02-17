// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/ScriptInterface.h"
#include "Interfaces/TurnBasedStrategist.h"
#include "TurnBasedCombatSubsystem.generated.h"

class UObject;
class AFight;
class UBAProfile;
class ACharacterSpawner;
class AAIController;
class UFighterProfile;
class UTurnBasedCombatConfig;

/**
 *
 */
UCLASS(Config = BionicApe)
class TURNBASEDCOMBAT_API UTurnBasedCombatSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient)
	TMap<UBAProfile*, AActor*> ExplorationActors;

	UPROPERTY(Transient, VisibleAnywhere)//TODO: If we find a better way to do this without using proxy then it won't be transient anymore
	UTurnBasedCombatConfig* Config; 
	UPROPERTY(Config)
	TSoftObjectPtr<UTurnBasedCombatConfig> ConfigProxy;

	
public:

	UTurnBasedCombatSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	AActor* GetExplorationActor(UBAProfile* Profile) const;

	void ProfileStartFight(UFighterProfile* Profile, AFight* Fight);
	
	void StartExplorationMode(UBAProfile* Profile, UObject* MyContext, bool const bIsDead, FTransform const CurrentTransform);

	ACharacterSpawner* FindSpawnerActor(UWorld* World, UBAProfile* Profile);
};
