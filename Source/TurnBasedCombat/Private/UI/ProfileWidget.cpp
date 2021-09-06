// Created by Bionic Ape. All Rights Reserved.


#include "UI/ProfileWidget.h"

#include "FighterProfile.h"

#include "Components/TextBlock.h"

#include "Components/TrpgControlComponent.h"

#include "Kismet/KismetTextLibrary.h"
#include "TurnBasedCombatLib.h"
#include "Components/ProgressBar.h"
#include "BAUISubsystem.h"
#include "UI/ConfirmWidget.h"
#include "BAUIConfig.h"
#include "Inventory/Inventory.h"


#define LOCTEXT_NAMESPACE "UProfileWidgetTextNamespace"

#pragma region AttributeButton

UAttributeButton::UAttributeButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	OnPressed.AddDynamic(this, &UAttributeButton::AddAttributePoint);
}

void UAttributeButton::AddAttributePoint()
{
	UE_LOG(LogTemp, Log, TEXT("UInventoryItemEntryListEntryStoreW::OnButtonPressed"));

	if (Profile && UBAUISubsystem::GetInstance() && UBAUISubsystem::GetInstance()->UIConfig && UBAUISubsystem::GetInstance()->UIConfig->ConfirmWidgetClass)
	{
		FText ConfirmText = FText::Format(LOCTEXT("AddAttributePointConfirm", "Do you want to add one point to Attribute {0}?"), FText::FromString(AttributeName));

		//UConfirmWidget* Confirm = UBAUISubsystem::CreateConfirm(this, LOCTEXT("OnLoginComplete_Successful", "You are successfully logged in"));

		UConfirmWidget* ConfirmWidget = CreateWidget<UConfirmWidget>(this, UBAUISubsystem::GetInstance()->UIConfig->ConfirmWidgetClass);
		ConfirmWidget->SetBodyText(ConfirmText);
		ConfirmWidget->AddToViewport();

		ConfirmWidget->OnFinishConfirmWidget.AddDynamic(this, &UAttributeButton::OnAddAttributePointConfirmed);
	}
}

void UAttributeButton::OnAddAttributePointConfirmed(bool bIsConfirmed)
{
	if (bIsConfirmed)
	{
		if (UTrpgControlComponent* TrpgControlComp = UTurnBasedCombatLib::GetTrpgControlComp(GetOwningPlayer()))
		{
			TrpgControlComp->AddAttributePoint(Profile, AttributeName);
		}
	}
}

#pragma endregion 

bool UProfileWidget::Initialize()
{
	bool bResult = Super::Initialize();
	if (bResult)
	{
		if (AddDexterityButton)
		{
			AddDexterityButton->AttributeName = TEXT("Dexterity");
			AddDexterityButton->Profile = Profile;
		}
		if (AddVitalityButton)
		{
			AddVitalityButton->AttributeName = TEXT("Vitality");
			AddVitalityButton->Profile = Profile;
		}

		if (AddStrengthButton)
		{
			AddStrengthButton->AttributeName = TEXT("Strength");
			AddStrengthButton->Profile = Profile;
		}

		if (AddAgilityButton)
		{
			AddAgilityButton->AttributeName = TEXT("Agility");
			AddAgilityButton->Profile = Profile;
		}

		if (AddIntelligenceButton)
		{
			AddIntelligenceButton->AttributeName = TEXT("Intelligence");
			AddIntelligenceButton->Profile = Profile;
		}
		if (AddCharismaButton)
		{
			AddCharismaButton->AttributeName = TEXT("Charisma");
			AddCharismaButton->Profile = Profile;
		}

		Refresh();
	}
	return bResult;
}

void UProfileWidget::SetProfile(UFighterProfile* NewProfile)
{

	if (Profile != NewProfile)//Are different?
	{
		if (Profile)
		{
			//Remove old delegates
			Profile->OnAttributesChanged.RemoveDynamic(this, &UProfileWidget::OnAttributesChanged);
			if (Profile->Inventory)
			{
				//remove old delegates
				Profile->Inventory->OnInventoryEntriesChanged.RemoveDynamic(this, &UProfileWidget::OnAttributesChanged);
				Profile->Inventory->OnInventoryCoinsChanged.RemoveDynamic(this, &UProfileWidget::OnAttributesChanged);
			}
		}
		Profile = NewProfile;//We set the new one!
		if (Profile)
		{
			//add new delegates
			Profile->OnAttributesChanged.AddDynamic(this, &UProfileWidget::OnAttributesChanged);
			if (Profile->Inventory)
			{
				//add new delegates
				Profile->Inventory->OnInventoryEntriesChanged.AddDynamic(this, &UProfileWidget::OnAttributesChanged);
				Profile->Inventory->OnInventoryCoinsChanged.AddDynamic(this, &UProfileWidget::OnAttributesChanged);
			}
		}

		{//Set Profile to Button Attributes
			if (AddDexterityButton)
			{
				AddDexterityButton->Profile = Profile;
			}
			if (AddVitalityButton)
			{
				AddVitalityButton->Profile = Profile;
			}
			if (AddStrengthButton)
			{
				AddStrengthButton->Profile = Profile;
			}
			if (AddAgilityButton)
			{
				AddAgilityButton->Profile = Profile;
			}
			if (AddIntelligenceButton)
			{
				AddIntelligenceButton->Profile = Profile;
			}
			if (AddCharismaButton)
			{
				AddCharismaButton->Profile = Profile;
			}
		}//End: Set Profile to Button Attributes

		Refresh();
	}
}

void UProfileWidget::Refresh()
{
	if (Profile)
	{
		if (IdTextBlock)
		{
			IdTextBlock->SetText(UKismetTextLibrary::Conv_StringToText(Profile->Id));
		}
		if (ProfileNameTextBlock)
		{
			ProfileNameTextBlock->SetText(UKismetTextLibrary::Conv_StringToText(Profile->ProfileName));
		}
		if (ActionPointsTextBlock)
		{
			ActionPointsTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(Profile->ActionPoints));
		}
		if (ActionsPerTurnTextBlock)
		{
			ActionsPerTurnTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(Profile->ActionsPerTurn));
		}
		//XP
		if (XpTextBlock)
		{
			XpTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(Profile->Attributes.Xp));
		}
		if (XpToLevelUpTextBlock)
		{
			XpToLevelUpTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(100 - (Profile->Attributes.Xp % 100)));
		}
		if (XPLevelTextBlock)
		{
			XPLevelTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(Profile->Attributes.XPLevel));
		}
		if (XpProgressBar)
		{
			XpProgressBar->SetPercent(Profile->Attributes.GetPercentToNextLevel());
		}
		//End XP

		if (CreationTimeTextBlock)
		{
			CreationTimeTextBlock->SetText(UKismetTextLibrary::AsDateTime_DateTime(Profile->CreationTime));
		}

		//Attributes
		if (AttributePointsTextBlock)
		{
			AttributePointsTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(Profile->Attributes.AttributePoints));
		}
		if (FelonyTextBlock)
		{
			FelonyTextBlock->SetText(UEnum::GetDisplayValueAsText(Profile->Attributes.Felony));
		}
		if (CharismaTextBlock)
		{
			CharismaTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(Profile->Attributes.Charisma));
		}
		if (DexterityTextBlock)
		{
			DexterityTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(Profile->Attributes.Dexterity));
		}
		if (IntelligenceTextBlock)
		{
			IntelligenceTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(Profile->Attributes.Intelligence));
		}
		if (AgilityTextBlock)
		{
			AgilityTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(Profile->Attributes.Agility));
		}
		if (VitalityTextBlock)
		{
			VitalityTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(Profile->Attributes.Vitality));
		}
		if (StrengthTextBlock)
		{
			StrengthTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(Profile->Attributes.Strength));
		}
		//End Attributes
		//Inventory
		if (CoinsTextBlock && Profile->Inventory)
		{

			CoinsTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(Profile->Inventory->GetCoins()));

		}
		//End Inventory
	}
}

void UProfileWidget::OnAttributesChanged()
{
	Refresh();
}

#undef LOCTEXT_NAMESPACE