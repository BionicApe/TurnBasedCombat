// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActionRequest.generated.h"

class UFocusableFilter;

/**
 * 
 */
UCLASS()
class TURNBASEDCOMBAT_API UActionRequest : public UObject
{
	GENERATED_BODY()

public:

	TArray<UFocusableFilter*> Filters;

public:


};
