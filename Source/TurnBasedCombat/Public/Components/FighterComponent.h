//// Created by Bionic Ape. All Rights Reserved.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
//#include "FighterComponent.generated.h"
//
//class UFighter;
//class APawn;
//class UTeam;
//
//
//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
//class TURNBASEDCOMBAT_API UFighterComponent : public UActorComponent
//{
//	GENERATED_BODY()
//
//public:	
//	
//	UPROPERTY(Replicated, Category = TurnBasedCombat, VisibleAnywhere, BlueprintReadOnly)
//	UTeam* Team;
//
//public:
//
//	// Sets default values for this component's properties
//	UFighterComponent();
//
//	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
//
//	UFUNCTION(Exec)
//	void CreateFight(UFighterComponent* EnemyCombatComp);
//
//	UFUNCTION(Server, Reliable, WithValidation)
//	void Server_CreateFight(UFighterComponent* EnemyPawnCombatComp);
//
//	APawn* GetPawnOwner() const { return Cast<APawn>(GetOwner()); }
//
//	UTeam* GetTeam() const;
//};
