// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProfileWidget.generated.h"

class UTextBlock;

/**
 *
 */
UCLASS()
class TURNBASEDCOMBAT_API UProfileWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UFighterProfile* Profile;

public:

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* IdTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* ProfileNameTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* ActionPointsTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* ActionsPerTurnTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* CreationTimeTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* XpTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* CharismaTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* DexterityTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* IntelligenceTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* AgilityTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* FelonyTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* XPLevelTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* VitalityTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* StrengthTextBlock;

public:

	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void SetProfile(UFighterProfile* NewProfile);

	UFUNCTION(BlueprintCallable)
	void Refresh();

	UFUNCTION(BlueprintCallable)
	void AddAttributePoint(FString AttributeName);

	UFUNCTION()
	void OnAttributesChanged();
	
	UFUNCTION()
	void OnFinishConfirmWidget(bool bIsConfirmed, FString AttributeName);
};
