// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "CharacterSpawner.generated.h"

class USceneComponent;
class UBillboardComponent;

UCLASS()
class TURNBASEDCOMBAT_API ACharacterSpawner : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	int32 ProfileId;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootSceneComp;
	
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBillboardComponent* BillBoardComp;

public:

	ACharacterSpawner();

};
