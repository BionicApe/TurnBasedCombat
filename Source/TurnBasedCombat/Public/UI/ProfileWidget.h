// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"

#include "ProfileWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UFighterProfile;
class UBARPGPersona;
class UBARPGAttribute;

#pragma region AttributeButton

/**
 *
 */
UCLASS()
class TURNBASEDCOMBAT_API UAttributeButton : public UButton
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AttributeName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UFighterProfile* Profile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBARPGPersona* Persona;

public:

	UAttributeButton(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void AddAttributePoint();
	
	UFUNCTION()
	void OnAddAttributePointConfirmed(bool bIsConfirmed);
};

#pragma endregion

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBARPGPersona* Persona;

public:

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBARPGAttribute* ExperienceAttributeKey;*/

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
	UTextBlock* AttributePointsTextBlock;

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
	UTextBlock* VitalityTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* StrengthTextBlock;

	//XP
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* XpTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* XPLevelTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* XpToLevelUpTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UProgressBar* XpProgressBar;
	
	//Inventory
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* CoinsTextBlock;

#pragma region AttributesButtons
	
	UPROPERTY(meta = (BindWidgetOptional))
	UAttributeButton* AddDexterityButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UAttributeButton* AddVitalityButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UAttributeButton* AddStrengthButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UAttributeButton* AddAgilityButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UAttributeButton* AddIntelligenceButton;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UAttributeButton* AddCharismaButton;


#pragma endregion
	

public:

	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void SetProfile(UFighterProfile* NewProfile);

	UFUNCTION(BlueprintCallable)
	void SetPersona(UBARPGPersona* NewPersona);

	UFUNCTION(BlueprintCallable)
	void Refresh();

	UFUNCTION()
	void OnAttributesChanged();
};
