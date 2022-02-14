// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrpgCombatTypes.h"
#include "UObject/CoreNet.h"
#include "Fight.generated.h"

class UBAProfile;
class ACombatant;
class UTeam;
class AArena;
class ITurnBasedStrategist;
class UActionType_Trpg;


USTRUCT(BlueprintType)
struct TURNBASEDCOMBAT_API FFightTurn
{
	GENERATED_USTRUCT_BODY()

	//Future Turn Modifiers here

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TRPG)
	UBAProfile* Profile;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TRPG)
	ACombatant* Combatant;

	//TODO: See if we will have a problem for not having a UPROPERTY with the Garbage collector
	ITurnBasedStrategist* Strategist;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TRPG)
	//UActionType_Trpg* LastActionPerformed;

	FFightTurn() : Profile(nullptr), Combatant(nullptr)
	{

	}
};



UCLASS(Config = BionicApe)
class TURNBASEDCOMBAT_API AFight : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = TurnBasedCombat, EditAnywhere, BlueprintReadOnly)
	TMap<UBAProfile*, ACombatant*> Combatants;

	UPROPERTY(Replicated, Category = TurnBasedCombat, EditAnywhere, BlueprintReadWrite)
	TArray<UTeam*> Teams;

	UPROPERTY(Replicated, Category = TurnBasedCombat, EditAnywhere, BlueprintReadWrite)
	AArena* Arena;

	//WARNING: I Suspect I am having "Blueprint could not be loaded because it derives from an invalid class" because I was using "Config" here in the header, and this class is from a module that depends on this module
	//WARNING 2: I moved this to CombatantClassProxy
	UPROPERTY(EditAnywhere/*, Config*/)
	TSubclassOf<ACombatant> CombatantClass;

	//WARNING: See warning on CombatantClass, let's try with SoftClassPtr just in case loading the asset causes to make it "derives from an invalid class"!
	UPROPERTY(Config)
	TSoftClassPtr<ACombatant> CombatantClassProxy;

	UPROPERTY(Category = TurnBasedCombat, VisibleAnywhere, Transient, Replicated)
	TArray<FFightTurn> Turns;

	UPROPERTY(Category = TurnBasedCombat, VisibleAnywhere, Transient, Replicated)
	ETurnState TurnState;

	UPROPERTY(Category = TurnBasedCombat, VisibleAnywhere, Transient, Replicated)
	EFightState FightState;

	UPROPERTY(Category = TurnBasedCombat, VisibleAnywhere, Transient, Replicated)
	int32 TurnIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrpgCombat, BlueprintAssignable)
	FOnTrpgCombatLog OnCombatLog;

	UPROPERTY(Transient, Replicated)
	FTrpgFightResults FightResults;

	UPROPERTY(EditAnywhere)
	float WaitTimeToTerminate = 5.f;

public:
	// Sets default values for this actor's properties
	AFight();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void AddTeam(UTeam* Team);
	
	bool IsCombatantTurn(ACombatant* Combatant) const;

	UFUNCTION(BlueprintCallable,Exec)
	void StartFight();

	FFightTurn const* GetCurrentFightTurn() const { return Turns.IsValidIndex(TurnIndex) ? &Turns[TurnIndex] : nullptr; }

	UFUNCTION()
	void TestDelayNotifyTurn();

	void NotifyNextTurn();

	bool AreEnemies(UBAProfile* ProfileA, UBAProfile* ProfileB) const;

	void SetTurnState(ETurnState NewTurnState);

	bool PerformAction(UActionType* Action, ITurnBasedStrategist* Strategist, ACombatant* Sender, ACombatant* Receiver);

	void CommitAction(FTrpgPerformActionRequest Request);

	void FinishTurn(ITurnBasedStrategist* Strategist, ACombatant* Combatant);

	//UTeam* GetTeam(UFighterComponent* FighterComp); 
	bool CheckCombatHasFinished();

	void TerminateFight();

private:

	bool PayAction(UActionType_Trpg const* TrpgAction, ACombatant* Combatant);

};

