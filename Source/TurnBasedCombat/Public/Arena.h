// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arena.generated.h"

class UActionType;
class ALevelSequenceActor;
class UBillboardComponent;
class UFighterComponent;
class AFight;
class AActionPerformer;
class IArenaPerformer;
class UArenaSequences;
class AArenaLevelSequenceActor;

USTRUCT(BlueprintType)
struct TURNBASEDCOMBAT_API FArenaPerformance
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrpgCombat)
	AActor* Sender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrpgCombat)
	AActor* Receiver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrpgCombat)
	UActionType* Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrpgCombat)
	int32 RandomNumber = 0;

	FArenaPerformance() : Sender(nullptr), Receiver(nullptr), Action(nullptr)
	{
		RandomNumber = FMath::Rand();
	}

	bool AreSenderAndReceiverTheSame() const { return Sender == Receiver; }
};

UCLASS()
class TURNBASEDCOMBAT_API AArena : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(Replicated, Transient, BlueprintReadOnly, Category = Combat)
	AFight* Fight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Combat)
	UArenaSequences* ArenaSequences;

	UPROPERTY(ReplicatedUsing = OnRep_ArenaPerformance, Transient, BlueprintReadOnly, Category = Combat)
	FArenaPerformance ArenaPerformance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	AActor* SequenceRootActor;

	UPROPERTY(/*Replicated, */Transient)
	TMap<UActionType const*, AArenaLevelSequenceActor*> SequencesSpawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	TSubclassOf<AActor> SelectionCircleClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = TrpgCombat)
	TArray<TSoftObjectPtr<AActor>> SelectionCircleArray;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBillboardComponent* OrbitCameraLocation;

	//TeamLocations
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBillboardComponent* TeamA_01;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBillboardComponent* TeamA_02;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBillboardComponent* TeamA_03;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBillboardComponent* TeamB_01;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBillboardComponent* TeamB_02;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBillboardComponent* TeamB_03;

	//TeamLocations

protected:

	virtual void BeginPlay() override;

public:

	AArena();

	virtual void Reset() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void StartNewFight(AFight* Fight);
	float PerformActionSequence(UActionType* Action, AActor* Sender, AActor* Receiver);
	void SetSelectedActor(AActor* SelectedActor);
	void RemoveSelectedActors();

	bool IsAvailableForANewFight() const { return Fight == nullptr; }

	UFUNCTION()
	void OnRep_ArenaPerformance();

	UFUNCTION()
	void OnLevelSequenceFinished();

	//UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnPerformAction"))
	//void ReceivePerformAction(FArenaPerformance const& Request);

	UFUNCTION(BlueprintCallable, Category = "TrpgCombat")
	const FTransform& GetPawnTransform(int32 TeamIndex, int32 WarriorIndex) const;

	const FTransform& GetOrbitCameraTransform() const;

	void AssignTransformsToFighters();

	//void AssignTransformToFighter(UFighterComponent* FighterComp);
};
