// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FocusInteractionsTypes.h"
#include "Interfaces/TurnBasedStrategist.h"
#include "Interfaces/InventoryOwner.h"
#include "PlayerCombatPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateChanged);

class USpringArmComponent;
class UStaticMeshComponent;
class UCameraComponent;
class UFocusTracerCursorComponent;
class UFocusableComponent;
class AArena;
class ACombatant;
class APlayerController;
class AFight;
class IStrategistHUD;
class UFighterProfile;


UCLASS(Config = BionicApe)
class TURNBASEDCOMBAT_API APlayerCombatPawn : public APawn, public ITurnBasedStrategist, public IInventoryOwner
{
	GENERATED_BODY()

public:

	static const FInputModeGameAndUI GameModeAndUIInputMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;

	UPROPERTY(Replicated)
	ACombatant* Combatant;

	UPROPERTY(Replicated)
	UBAProfile* Profile;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Fight)
	AFight* Fight;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_IsMyTurn)
	bool bIsMyTurn = false;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_IsMyTurn)
	bool bHackIsCombatFinished = false;

	IStrategistHUD* StrategistHUD;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnStateChanged OnTurnStarted;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnStateChanged OnCombatFinished;




public:
	
	APlayerCombatPawn();

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Restart() override;

	virtual void Tick(float DeltaSeconds) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	APlayerController* GetPlayerController() const;	

	virtual void BeginDestroy() override;

	UFUNCTION()
	void OnRep_Fight();
	
	UFUNCTION()
	void OnRep_Combatant();

	UFUNCTION()
	void OnRep_IsMyTurn();

#pragma region IInventoryOwner
	void SetSelectedItem(UInventoryItem* InventoryItem) override;
	virtual UInventory* GetInventory() const override;
	virtual void GetInventoryItemsList(TArray<UInventoryItem*>& MyInventoryItems) const override;
	
	//Not from IInventoryOwner
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetSelectedItem(UInventoryItem* InventoryItem);

	UFUNCTION(BlueprintCallable)
	void RequestFinishTurn();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestFinishTurn();

#pragma endregion
	
#pragma region Input

public:

	UPROPERTY(EditAnywhere)
	float TurnRate = 45.f;

	UPROPERTY(EditAnywhere)
	float TurnSesibility = 1.f;

	UPROPERTY(EditAnywhere)
	float LookUpRate = 45.f;

	UPROPERTY(EditAnywhere)
	float LookUpSesibility = -1.f;

	UPROPERTY(EditAnywhere)
	float ZoomRate = -10.f;//Negative numbers for Expected Mouse Wheel

	UPROPERTY(EditAnywhere)
	float MinZoom = 400.f;
	
	UPROPERTY(EditAnywhere)
	float MaxZoom = 1500.f;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Turn(float Rate);
	void TurnAtRate(float Rate);
	void LookUp(float Rate);
	void LookUpAtRate(float Rate);
	void ZoomIn(float Rate);
	void TrpgMouse1();
	void TrpgMouse2();
#pragma endregion

#pragma region FocusTracer

	UPROPERTY(Category = TrpgCombat, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UFocusTracerCursorComponent* FocusTracer;

	UFUNCTION()
	void OnNewFocus(const FFocusTraceInfo& Info);
	UFUNCTION()
	void OnEndFocus(const UFocusableComponent* Focusable);
	UFUNCTION()
	void OnNewActionsSets();
#pragma endregion

#pragma region ITurnBasedStrategist
	virtual void StartFight(AFight* NewFight) override;
	virtual void StartTurn(UBAProfile* FighterProfile, ACombatant* NewCombatant) override;
	virtual void EndTurn() override;
	UBAProfile* GetCurrentProfile() override { return Profile;}
	void NotifyFightFinish(AFight* FinishedFight) override;
	AFight* GetFight() override {return Fight;}
#pragma endregion
	
	UFUNCTION(Client, Reliable)
	void Client_NotifyFightFinish();

private:
	bool TryToSetHUD();
};
