// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "ArenaPerformer.h"
#include "ArenaPerformerSkeletal.generated.h"

class AActor;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API AArenaPerformerSkeletal : public ASkeletalMeshActor, public IArenaPerformer
{
	GENERATED_BODY()

public:

	void Setup(AActor* Actor) override;

	void Teardown() override;

};
