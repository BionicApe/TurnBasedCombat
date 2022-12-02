// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/CoreNet.h"
#include "Interfaces/ProfileAsignable.h"
#include "DamageSystemTypes.h"
#include "Engine/EngineTypes.h"
#include "Misc/Guid.h"
#include "TrpgCombatTypes.h"
#include "Combatant.generated.h"

class UInventoryComponent;
class UBAProfile;
class UHealthComponent;
class UFocusableComponent;
class UInventoryItem;
class UFighterProfile;
class AFight;
class UActionType;
class ACombatant;
class UActionType_Trpg;

USTRUCT(BlueprintType)
struct TURNBASEDCOMBAT_API FCombatantLastAction
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	const UActionType_Trpg* Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInventoryItem* Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACombatant* Target;

	FCombatantLastAction(const UActionType_Trpg* Action, UInventoryItem* Item, ACombatant* Target) : Action(Action), Item(Item), Target(Target)
	{

	}
	FCombatantLastAction() : Action(nullptr), Item(nullptr), Target(nullptr)
	{

	}

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombatantDIeDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFightSetUp, AFight*, Fight);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDefaultItemSelected, UInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionPointsChange,unsigned int, ActionPoints);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemainingSecondsChange, unsigned int, RemainingSeconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemainActionsChange, unsigned int, RemainingActions);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatantStartAction, FCombatantLastAction, LastActionInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnUpdate, bool, MyTurn);

UCLASS(BlueprintType, Blueprintable)
class TURNBASEDCOMBAT_API ACombatant : public AActor, public IProfileAsignable
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = TurnBasedCombat, EditAnywhere, ReplicatedUsing = "OnRep_Fight")
	AFight* Fight;

	UPROPERTY(Category = TurnBasedCombat, EditAnywhere, Replicated)
	UFighterProfile* Profile;

	UPROPERTY(Replicated)
	FGuid ID;

	UPROPERTY(Category = TurnBasedCombat, EditAnywhere, ReplicatedUsing = "OnRep_ActionPoints")
	unsigned int ActionPoints;

	UPROPERTY(Category = TurnBasedCombat, EditAnywhere, ReplicatedUsing = "OnRep_RemainingActions")
	unsigned int RemainingActions = 2;

	UPROPERTY(BlueprintReadWrite, Category = TrpgCombat, ReplicatedUsing = "OnRep_RemainingSeconds")
	float RemainingSeconds = 60;

	UPROPERTY()
	FTimerHandle TurnTimerHandle;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_IsMyTurn")
	bool bIsMyTurn = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool PerformingAction = false;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnActionPointsChange OnActionPointsChange;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnRemainActionsChange OnRemainActionsChange;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnTurnUpdate OnTurnUpdate;	

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnFightSetUp OnFightSetUp;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnRemainingSecondsChange OnRemainingSecondsChange;

	UPROPERTY(Replicated)
	FCombatantLastAction LastAction;

	UPROPERTY()
	FOnCombatantStartAction OnCombatantStartAction;

	UPROPERTY()
	FOnDefaultItemSelected OnDefaultItemSelected;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Defaults|Item", Replicated)
	UInventoryItem* DefaultInventoryItem = nullptr;

public:

	ACombatant();

	virtual void Tick (float DeltaSeconds) override;

	//virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor * DamageCauser) override;
	//virtual float Heal(float HealAmount);
	virtual void SetBAProfile(UBAProfile* Profile) override;
	virtual UBAProfile* GetBAProfile() const override;

	void GenerateID();

	UFUNCTION()
	bool CanUseThisItem(UInventoryItem* Item);
	bool HasEnoughActionPoints(unsigned int Value);
	bool HasEnoughActions();
	/**
	* @brief Validates the action, if my selected item has this action, has enough action points and actions.
	* @see CanUseThisItem
	* @see HasEnoughActionPoints
	* @see HasEnoughActions
	*
	* @param Action: Current action to perform
	* @return True if my selected item has the action.
	*/
	bool ValidateAction(const UActionType_Trpg* Action, FTrpgPerformActionRequest Request, FTrpgPerformActionResult& Result);
	/**
	 * @brief Substract the action points and actions. And use the item.
	 * 
	 * @param Action:
	 */
	void PayActionWithoutValidation(const UActionType_Trpg* Action);
	/**
	 * @brief Substract the action points and actions and use the item if is possible.
	 * @see ValidateAction
	 * @see PayActionWithoutValidation
	 * 
	 * @param Action:
	 * @param Request:
	 * @param Result: 
	 * @return true->The action was perform, false is not possible to perform this action
	 */
	bool PayActionWithValidation(const UActionType_Trpg* Action, FTrpgPerformActionRequest Request, FTrpgPerformActionResult& Result);

	/**
	 * @brief Substract action points if possible
	 * 
	 * @param Value:Quantity of AP to substract.
	 * @return true if substracted.
	 */
	bool TryConsumeActionPoints(unsigned int Value);
	/**
	 * @brief Substracts remainin actions if possible
	 * 
	 * @return true if substracted.
	 */
	bool TryPerformAction();

	UFUNCTION()
	virtual void OnNewItemSelected(UInventoryItem* InventoryItem);

	virtual void StartTurnUpdateValues();

	virtual void EndTurnUpdateValues();

	UFUNCTION()
	virtual void OnDie(FActorKilled ActorKilledProperties);
	
	UFUNCTION()
	virtual void OnTakeDamage(FTakeDamageProperties const TakeDamageProperties);

	virtual bool IsDead();

	virtual bool IsMyTurn() { return bIsMyTurn; };

	UFUNCTION()
	virtual void OnRep_IsDead();

	UFUNCTION()
	virtual void OnRep_ActionPoints();

	UFUNCTION()
	virtual void OnRep_RemainingActions();

	UFUNCTION()
	void OnRep_Fight();

	UFUNCTION()
	void OnRep_IsMyTurn();
	UFUNCTION()
	void OnRep_RemainingSeconds();
	UFUNCTION()
	void TimerEnds();

	UFUNCTION()
	/**
		* @brief Called to tell the combatant that it is performing an action. The timer must stop.
		* 
		* 
		*/
	void StartPerformingAction(const UActionType_Trpg* Action, ACombatant* Target);

	UFUNCTION()
	/**
		* @brief Called when the action animation ends. Timer must resume.
		* 
		*/
	void EndPerformingAction();

	void SubstractActionPoints(unsigned int Value);
	void SubstractActions(unsigned int Value = 1);

	UFUNCTION(Server, Reliable)
	virtual void Server_SetDefaultInventoryItem(UInventoryItem* NewDefaultInventoryItem);

	FCombatantLastAction GetLastActionInfo();
};
