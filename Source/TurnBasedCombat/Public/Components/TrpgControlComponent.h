// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControlComponent.h"
#include "Interfaces/BAMultiplayerDAO.h"
#include "TrpgControlComponent.generated.h"

class UFighterProfile;
class AMockupFocusable;
class APlayerController;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TURNBASEDCOMBAT_API UTrpgControlComponent : public UControlComponent
{
	GENERATED_BODY()

protected:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", Replicated)
	TArray<UFighterProfile*> Profiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", Replicated)
	UFighterProfile* MainProfile;

public:


	//UFUNCTION(BlueprintCallable)
	//APlayerController* GetController() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UFUNCTION(BlueprintCallable)
	void AddProfile(UFighterProfile* NewFighterProfile, bool bIsMainInventory);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_ResponseReceived(bool bIsSuccessful, const FString& Message);

	UFUNCTION(BlueprintCallable)
	virtual UFighterProfile* GetMainFighterProfile() const { return MainProfile; }
	
	UFUNCTION(BlueprintCallable)
	void AddXP(UFighterProfile* FighterProfile, AMockupFocusable* MockupFocusable);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_AddXP(UFighterProfile* FighterProfile, AMockupFocusable* MockupFocusable);
	UFUNCTION()
	void OnUpdateProfile(FBAProfileResponse Response);

	UFUNCTION(BlueprintCallable)
	void AddAttributePoints(UFighterProfile* FighterProfile, AMockupFocusable* MockupFocusable);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_AddAttributePoints(UFighterProfile* FighterProfile, AMockupFocusable* MockupFocusable);
};
