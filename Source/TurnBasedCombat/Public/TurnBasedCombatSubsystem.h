// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/ScriptInterface.h"
#include "Interfaces/TurnBasedStrategist.h"
#include "AIController.h"
#include "TurnBasedCombatSubsystem.generated.h"

class UPrisonerProfile;
class UObject;
class AFight;
class UBAProfile;
class ACharacterSpawner;
class AAIController;

/**
 *
 */
UCLASS(Config = BionicApe)
class TURNBASEDCOMBAT_API UTurnBasedCombatSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(Config)
	TSubclassOf<UObject> AIStrategistClass;

	UPROPERTY(Config)
	TSubclassOf<APawn> PlayerCombatPawnClass;

	UPROPERTY(Config)
	TSubclassOf<APawn> ExplorationPawnClass;

	UPROPERTY(Config)
	TSubclassOf<AAIController> NpcAiControllerClass;

	/**
	 * Used to avoid being garbage collected
	 */
	UPROPERTY(Transient)
	UObject* AIStrategistObj;
	ITurnBasedStrategist* AIStrategist;
		
	UPROPERTY(Transient)
	TMap<UBAProfile*, TScriptInterface<ITurnBasedStrategist>> StrategistsAssigned;//Check IsValid(Pointer))

	UPROPERTY(Transient)
	TMap<UBAProfile*, AActor*> ExplorationActors;

	
public:

	UTurnBasedCombatSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	ITurnBasedStrategist* GetStrategist(UBAProfile* Profile) const;

	AActor* GetExplorationActor(UBAProfile* Profile) const;

	void AddStrategist(UBAProfile* Profile, ITurnBasedStrategist* Strategist);

	void RemoveStrategist(ITurnBasedStrategist* Strategist);

	void ProfileStartFight(UBAProfile* Profile, AFight* Fight);
	
	void StartExplorationMode(UBAProfile* Profile, UObject* MyContext, bool const bIsDead, FTransform const CurrentTransform);

	ACharacterSpawner* FindSpawnerActor(UWorld* World, UBAProfile* Profile);
};
