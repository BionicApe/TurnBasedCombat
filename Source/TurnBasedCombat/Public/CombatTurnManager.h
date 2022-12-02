// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatTurnManager.generated.h"


class UActionType;
class ITurnBasedStrategist;
class ACombatant;

USTRUCT(BlueprintType)
struct TURNBASEDCOMBAT_API FTurnActionHistoryItem
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	const UActionType* Action;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ITurnBasedStrategist* Strategist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACombatant* Sender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACombatant* Receiver;

	FTurnActionHistoryItem(const UActionType* Action, ITurnBasedStrategist* Strategist, ACombatant* Sender, ACombatant* Receiver) : Action(Action), Strategist(Strategist), Sender(Sender), Receiver(Receiver)
	{

	}
	FTurnActionHistoryItem() : Action(nullptr), Strategist(nullptr), Sender(nullptr), Receiver(nullptr)
	{

	}

};

//It is an actor because it has to be replicated.
UCLASS()
class TURNBASEDCOMBAT_API ACombatTurnManager : public AActor
{
	GENERATED_BODY()
	
#pragma region Attributes
public:
protected:
	TArray<FTurnActionHistoryItem> ActionsHistory;
private:
#pragma endregion Attributes

#pragma region Methods
public:
	// Sets default values for this actor's properties
	ACombatTurnManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddActionToHistory(FTurnActionHistoryItem Item);
	TArray<FTurnActionHistoryItem> GetActionsHistory();
	FTurnActionHistoryItem const* GetActionHistory(int Index);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
#pragma endregion Methods 


};
