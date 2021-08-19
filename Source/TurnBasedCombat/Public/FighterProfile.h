// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BAProfile.h"
#include "Interfaces/InventoryOwner.h"
#include "FighterProfile.generated.h"

class UInventory;
class UInventoryItem;
class UTeam;

UENUM(BlueprintType)
enum class EFelonyType : uint8
{
	//Asesino a sueldo
	HITMAN				UMETA(DisplayName = "Hitman"),
	//Ladrón
	THIEF				UMETA(DisplayName = "Thief"),
	//Hacker
	HACKER				UMETA(DisplayName = "Hacker"),
	//Asesino en serie
	SERIAL_KILLER		UMETA(DisplayName = "Serial Killer"),
	//Trafficker
	TRAFFICKER			UMETA(DisplayName = "Trafficker"),
	//Timador
	HUSTLER				UMETA(DisplayName = "Hustler"),
	//Terrorist
	TERRORIST			UMETA(DisplayName = "Terrorist")
};


USTRUCT(BlueprintType)
struct TURNBASEDCOMBAT_API FFighterAttributes
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EFelonyType Felony;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 XPLevel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Xp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 AttributePoints;

	//Original Game = Fuerza
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Strength;

	//Original Game = Destreza
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Dexterity;

	//Original Game = Agilidad
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Agility;

	//Original Game = Constitucion
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Vitality;

	//Original Game = Inteligencia
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Intelligence;

	//Original Game = Carisma
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Charisma;
	
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProfileActionPointsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionsPerTurnChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttributesChanged);


/**
 *
 */
UCLASS()
class TURNBASEDCOMBAT_API UFighterProfile : public UBAProfile, public IInventoryOwner
{
	GENERATED_BODY()


#pragma region Property_Inventory	
public:
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly)
	UInventory* Inventory;
public:
	UFUNCTION(BlueprintCallable)
	virtual UInventory* GetInventory() const override { return Inventory; }
	UFUNCTION(BlueprintCallable)
	void SetInventory(UInventory* NewInventory) { Inventory = NewInventory; }
	//IInventoryOwner
	virtual void SetSelectedItem(UInventoryItem* InventoryItem) {} //TO DELETE!
	virtual void GetInventoryItemsList(TArray<UInventoryItem*>& MyInventoryItems) const {} //TO DELETE!
	//End IInventoryOwner
#pragma endregion

#pragma region Property_Team
protected:
	
	UPROPERTY(Replicated, EditDefaultsOnly)
	UTeam* Team;
public:
	UFUNCTION(BlueprintCallable)
	UTeam* GetTeam()const { return Team; }
	UFUNCTION(BlueprintCallable)
	void SetTeam(UTeam* NewTeam) { Team = NewTeam; }
#pragma endregion

#pragma region Property_ActionPoints
public:
	UPROPERTY(ReplicatedUsing = "OnRep_ActionPoints", EditDefaultsOnly, BlueprintReadOnly)
	int32 ActionPoints = 100;
	UPROPERTY(Transient)
	FOnProfileActionPointsChanged OnProfileActionPointsChanged;

	UFUNCTION(BlueprintCallable)
	int32 GetActionPoints() const { return ActionPoints; }
	UFUNCTION(BlueprintCallable)
	void SetActionPoints(int32 NewActionPoints);
	UFUNCTION()
	void OnRep_ActionPoints();

#pragma endregion
#pragma region Property_ActionsPerTurn
public:
	UPROPERTY(BlueprintReadWrite, Category = TrpgCombat)
	int32 ActionsPerTurn = 2;
	UPROPERTY(Transient)
	FOnActionsPerTurnChanged OnActionsPerTurnChanged;
public:
	UFUNCTION(BlueprintCallable)
	int32 GetActionsPerTurn() const { return ActionsPerTurn; }
	UFUNCTION(BlueprintCallable)
	void SetActionsPerTurn(int32 NewActionsPerTurn);
	UFUNCTION()
	void OnRep_ActionsPerTurn();
#pragma endregion

#pragma region Property_Xp
public:

	UPROPERTY(ReplicatedUsing="OnRep_Attributes", EditDefaultsOnly)
	FFighterAttributes Attributes;

	UPROPERTY(Transient)
	FOnAttributesChanged OnAttributesChanged;

public:
	UFUNCTION(BlueprintCallable)
	int32 GetXp() const { return Attributes.Xp; }
	UFUNCTION(BlueprintCallable)
	void SetAttributes(FFighterAttributes NewAttributes);
	UFUNCTION(BlueprintCallable)
	int32 AddXp(int32 XpToAdd);
	UFUNCTION(BlueprintCallable)
	int32 AddAttributePoints(int32 PointsToAdd);

	UFUNCTION()
	void OnRep_Attributes();
#pragma endregion

public:
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
