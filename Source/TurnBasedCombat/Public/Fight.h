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
class APawn;
class APlayerCombatPawn;
class UFighterProfile;

USTRUCT(BlueprintType)
struct TURNBASEDCOMBAT_API FFightCombatantsInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TRPG)
	FString CombatantName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TRPG)
	int32 Pos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TRPG)
	FGuid ID;

	FFightCombatantsInfo(FString CombatantName, int32 Pos, FGuid ID) : CombatantName(CombatantName), Pos(Pos), ID(ID)
	{

	}
	FFightCombatantsInfo() : CombatantName(""), Pos(0), ID()
	{

	}
};

USTRUCT(BlueprintType)
struct TURNBASEDCOMBAT_API FFightTurn
{
	GENERATED_USTRUCT_BODY()

	//Future Turn Modifiers here
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TRPG)
	UFighterProfile* Profile;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TRPG)
	ACombatant* Combatant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TRPG)
	FFightCombatantsInfo CombatantInfo;//For replication dont use Profiles

	////TODO: See if we will have a problem for not having a UPROPERTY with the Garbage collector
	//ITurnBasedStrategist* Strategist;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TRPG)
	//UActionType_Trpg* LastActionPerformed;

	FFightTurn() : Profile(nullptr), Combatant(nullptr), CombatantInfo()
	{

	}
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewTurnCombatant, FFightTurn, Combatant);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCombatantDieOrRevive, ACombatant*, Combatant, bool, IsDead);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewRound, TArray<FFightTurn>, Turns);

UCLASS(/*Config = BionicApe*/)
class TURNBASEDCOMBAT_API AFight : public AActor
{
	GENERATED_BODY()

protected:

	//WARNING: I Suspect I am having "Blueprint could not be loaded because it derives from an invalid class" because I was using "Config" here in the header, and this class is from a module that depends on this module
	//WARNING 2: Changed it so it's blueprint based instead of Config
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly/*, Config*/)
	TSubclassOf<ACombatant> CombatantClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly/*, Config*/)
	TSubclassOf<APlayerCombatPawn> PlayerCombatPawnClass;

	TMap<UBAProfile*, FTransform> SavedPositions;

public:

	UPROPERTY(Category = TurnBasedCombat, EditAnywhere, BlueprintReadOnly)
	TMap<UBAProfile*, ACombatant*> Combatants;

	UPROPERTY(Replicated, Category = TurnBasedCombat, EditAnywhere, BlueprintReadWrite)
	TArray<UTeam*> Teams;

	UPROPERTY(Replicated, Category = TurnBasedCombat, EditAnywhere, BlueprintReadWrite)
	AArena* Arena;

	UPROPERTY(Category = TurnBasedCombat, VisibleAnywhere, Transient, Replicated)
	TArray<FFightTurn> Turns;

	UPROPERTY(Category = TurnBasedCombat, VisibleAnywhere, Transient, Replicated)
	ETurnState TurnState;

	UPROPERTY(Category = TurnBasedCombat, VisibleAnywhere, Transient, Replicated)
	EFightState FightState;

	UPROPERTY(Category = TurnBasedCombat, VisibleAnywhere, Transient, ReplicatedUsing = "OnRep_TurnIndex")
	int32 TurnIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrpgCombat, BlueprintAssignable)
	FOnTrpgCombatLog OnCombatLog;

	UPROPERTY(Transient, Replicated)
	FTrpgFightResults FightResults;

	UPROPERTY(EditAnywhere)
	float WaitTimeToTerminate = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnNewTurnCombatant OnNewTurnCombatant;

	//UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	//FOnCombatantDieOrRevive OnCombatantDieOrRevive;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnNewRound OnNewRound;

public:
	// Sets default values for this actor's properties
	AFight();

protected:

	virtual void BeginPlay() override;

	/**
	 * @brief When all combatants perform their actions, new turns must be generated.
	 * 
	 */
	void NewRound();

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

	bool PerformAction(const UActionType* Action, ITurnBasedStrategist* Strategist, ACombatant* Sender, ACombatant* Receiver);

	void CommitAction(FTrpgPerformActionRequest Request);

	void FinishTurn(ITurnBasedStrategist* Strategist, ACombatant* Combatant);

	//UTeam* GetTeam(UFighterComponent* FighterComp); 
	bool CheckCombatHasFinished();

	void TerminateFight();

	UFUNCTION()
	void OnRep_TurnIndex();

	UFUNCTION(NetMulticast, Reliable)
		/**
		 * @brief Sync all the clients and notify that new round starts.
		 * 
		 * @param NTurns:
		 */
	void NetMulticast_OnNewRoundEvent(const TArray<FFightTurn>& NTurns);

	UFUNCTION(NetMulticast, Reliable)
	/**
		* @brief Sync all the clients and notify that a new turn starts.
		*
		* @param NTurns:
		*/
	void NetMulticast_OnNewTurnEvent(int Turn);

	//UFUNCTION(NetMulticast, Reliable)
	//	/**
	//	 * @brief 
	//	 * 
	//	 * @param Combatant
	//	 * @param IsDead
	//	 */
	//void NetMulticast_OnCombatantDieOrRevive(ACombatant* Combatant, bool IsDead);

	//UFUNCTION()
	//	/**
	//	 * @brief 
	//	 * 
	//	 * @param OldHealth:
	//	 * @param NewHealth:
	//	 * @param Owner:
	//	 */
	//void OnCombatantHealthChange(int32 OldHealth, int32 NewHealth, AActor* HealthOwner);


	UFUNCTION()
	TArray<FFightTurn> GetTurns();
private:

	bool PayAction(UActionType_Trpg const* TrpgAction, ACombatant* Combatant);

};

