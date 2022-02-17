// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "TrpgCombatTypes.generated.h"

//class UTrpgWarriorComponent;
class UObject;
class UActionType;
class AFight;
class UActionableType;
class UActionType_Trpg;
class UTeam;

UENUM(BlueprintType)
enum class ETurnState : uint8
{
	NO_TURN,
	ASSIGNING_TURN,
	CAN_RECEIVE_ACTIONS,
	PERFORMING_ACTION_STAGE,
	NOTIFYING_TURN_RESULT
};

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTurnStateChanged, ETurnState, TurnState, UTrpgWarriorComponent*, NewTurnWarrior);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnStarted, UTrpgWarriorComponent*, TurnWarrior);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnFinished, UTrpgWarriorComponent*, TurnWarrior);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnValuesUpdated, UTrpgWarriorComponent*, TurnWarrior);

UENUM(BlueprintType)
enum class EFightState : uint8
{
	INITIALIZING,
	FIGHTING,
	FINISHED
};

USTRUCT(BlueprintType)
struct TURNBASEDCOMBAT_API FTrpgCombatLog
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrpgCombat)
	TArray <FText> Messages;

	FTrpgCombatLog()
	{

	}

	FTrpgCombatLog(FText const& NewMessage)
	{
		Messages.Add(NewMessage);
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrpgCombatLog, const FTrpgCombatLog&, CombatLog);

USTRUCT(BlueprintType)
struct TURNBASEDCOMBAT_API FTrpgPerformActionResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrpgCombat)
	bool bIsSuccessful;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrpgCombat)
	FTrpgCombatLog Log;

	FTrpgPerformActionResult() : bIsSuccessful(false)
	{

	}
};

USTRUCT(BlueprintType)
struct TURNBASEDCOMBAT_API FTrpgPerformActionRequest
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrpgCombat)
	AActor* Sender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrpgCombat)
	AActor* Receiver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrpgCombat)
	const UActionType_Trpg* Action;

	FTrpgPerformActionRequest() : Sender(nullptr), Receiver(nullptr), Action(nullptr)
	{

	}

	bool AreSenderAndReceiverTheSame() const { return Sender == Receiver; }
};

UENUM(BlueprintType)
enum class EFightResultType : uint8
{
	HAS_A_WINNER = 0,
	ALL_DEFEATED = 1,
	INTERRUPTED = 2,
	YOU_FLED = 3
};

USTRUCT(BlueprintType)
struct TURNBASEDCOMBAT_API FTrpgFightResults
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrpgCombat)
	EFightResultType ResultType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrpgCombat)
	FString WinnerTeamName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrpgCombat)
	AFight* Fight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrpgCombat)
	UTeam* WinnerTeam;

	FTrpgFightResults() : ResultType(EFightResultType::HAS_A_WINNER), Fight(nullptr), WinnerTeam(nullptr)
	{
	}
};