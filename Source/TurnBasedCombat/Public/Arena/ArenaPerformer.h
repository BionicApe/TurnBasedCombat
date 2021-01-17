// Move 36 Studio

#pragma once

#include "UObject/Interface.h"
#include "ArenaPerformer.generated.h"


/**
*
*/
UINTERFACE(Blueprintable)
class TURNBASEDCOMBAT_API UArenaPerformer : public UInterface
{
	GENERATED_BODY()
};

class IArenaPerformer
{
	GENERATED_BODY()

public:

	virtual void Setup(AActor* Actor) = 0;
	virtual void Teardown() = 0;
};