// Created by Bionic Ape. All Rights Reserved.


#include "UI/ProfileWidget.h"

#include "FighterProfile.h"

#include "BARPGPersona.h"
#include "BARPGAttribute.h"
#include "BARPGModel.h"

#include "Components/TextBlock.h"

#include "Components/TrpgControlComponent.h"
#include "Components/BARPGControlComponent.h"
#include "Components/BaseRpgControlComponent.h"

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
		//if (UTrpgControlComponent* TrpgControlComp = UTurnBasedCombatLib::GetTrpgControlComp(GetOwningPlayer()))
		//{
		//	//TrpgControlComp->AddAttributePoint(Profile, AttributeName);
		//}

		if (UBaseRpgControlComponent* BARPGControlComp = Cast<UBaseRpgControlComponent>(GetOwningPlayer()->GetComponentByClass(UBARPGControlComponent::StaticClass())))
		{
			BARPGControlComp->ExpendAttributePoints(Persona, Persona->Model->AttributePointsAttributeKey, 1);
		}
	}
}

#pragma endregion 

bool UProfileWidget::Initialize()
{
	bool bResult = Super::Initialize();
	if (bResult)
	{
		if (AddDexterityButton) //TODO: REDO using dynamic information from BARPGModel
		{
			AddDexterityButton->AttributeName = TEXT("Dexterity");
			AddDexterityButton->Profile = Profile;
			AddDexterityButton->Persona = Persona;
		}
		if (AddVitalityButton)
		{
			AddVitalityButton->AttributeName = TEXT("Vitality");
			AddVitalityButton->Profile = Profile;
			AddVitalityButton->Persona = Persona;
		}

		if (AddStrengthButton)
		{
			AddStrengthButton->AttributeName = TEXT("Strength");
			AddStrengthButton->Profile = Profile;
			AddStrengthButton->Persona = Persona;
		}

		if (AddAgilityButton)
		{
			AddAgilityButton->AttributeName = TEXT("Agility");
			AddAgilityButton->Profile = Profile;
			AddAgilityButton->Persona = Persona;
		}

		if (AddIntelligenceButton)
		{
			AddIntelligenceButton->AttributeName = TEXT("Intelligence");
			AddIntelligenceButton->Profile = Profile;
			AddIntelligenceButton->Persona = Persona;
		}
		if (AddCharismaButton)
		{
			AddCharismaButton->AttributeName = TEXT("Charisma");
			AddCharismaButton->Profile = Profile;
			AddCharismaButton->Persona = Persona;
		}

		Refresh();
	}
	return bResult;
}

void UProfileWidget::SetPersona(UBARPGPersona* NewPersona)
{
	if (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer)
	{
		UE_LOG(LogTemp, Log, TEXT("I'm Server"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("I'm Client"));
	}

	if (Persona != NewPersona)//Are different?
	{
		if (Persona)
		{
			//Remove old delegates
			Persona->OnPersonaAttributesChanged.RemoveDynamic(this, &UProfileWidget::OnAttributesChanged);
		}
		Persona = NewPersona;//We set the new one!
		if (Persona)
		{
			//add new delegates
			Persona->OnPersonaAttributesChanged.AddDynamic(this, &UProfileWidget::OnAttributesChanged);
		}

		{//Set Persona to Button Attributes
			if (AddDexterityButton)
			{
				AddDexterityButton->Persona = Persona;
			}
			if (AddVitalityButton)
			{
				AddVitalityButton->Persona = Persona;
			}
			if (AddStrengthButton)
			{
				AddStrengthButton->Persona = Persona;
			}
			if (AddAgilityButton)
			{
				AddAgilityButton->Persona = Persona;
			}
			if (AddIntelligenceButton)
			{
				AddIntelligenceButton->Persona = Persona;
			}
			if (AddCharismaButton)
			{
				AddCharismaButton->Persona = Persona;
			}
		}//End: Set Persona to Button Attributes

		Refresh();
	}
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
	if (Profile && Persona)
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
			
		FBARPGAttributeValue XPAttValue = Persona->FindAttributeValueByKey(Persona->Model->ExperienceAttributeKey);
		//XP
		if (XpTextBlock)
		{
			XpTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(XPAttValue.Value));
		}
		
		FBARPGAttributeValue LVLAttValue = Persona->FindAttributeValueByKey(Persona->Model->LevelAttributeKey);
		int32 ExperienceRequired = Persona->Model->ExperienceRequiredPerLevel->GetFloatValue(LVLAttValue.Value);

		if (XpToLevelUpTextBlock)
		{
			XpToLevelUpTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(ExperienceRequired - XPAttValue.Value));
		}
		if (XPLevelTextBlock)
		{
			//XPLevelTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(Profile->Attributes.XPLevel));
			XPLevelTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(LVLAttValue.Value));
		}
		if (XpProgressBar)
		{
			XpProgressBar->SetPercent(((float)XPAttValue.Value / (float)ExperienceRequired)); //If this was a inner lop, oh boy

		}
		//End XP

		if (CreationTimeTextBlock)
		{
			CreationTimeTextBlock->SetText(UKismetTextLibrary::AsDateTime_DateTime(Profile->CreationTime));
		}

		//Attributes
		if (AttributePointsTextBlock)
		{
			//AttributePointsTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(Profile->Attributes.AttributePoints));
			int32 CurrentExpendedAttributePoints = Persona->FindAttributeValueByKey(Persona->Model->AttributePointsAttributeKey).Value;
			int32 MaxAttributePoints = Persona->Model->AttributePointsPerLevel->GetFloatValue(LVLAttValue.Value);
			AttributePointsTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(MaxAttributePoints - CurrentExpendedAttributePoints));
		}
		if (FelonyTextBlock)
		{
			FelonyTextBlock->SetText(UEnum::GetDisplayValueAsText(Profile->Attributes.Felony));
		}
		if (StrengthTextBlock)
		{
			
			FBARPGAttributeValue StrAttValue = Persona->FindAttributeValueByName(TEXT("Strength"));
			StrengthTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(StrAttValue.Value)); //Todo, switch to names/attributes
		}
		if (DexterityTextBlock)
		{
			FBARPGAttributeValue DexAttValue = Persona->FindAttributeValueByName(TEXT("Dexterity"));
			DexterityTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(DexAttValue.Value));
		}
		if (VitalityTextBlock)
		{
			FBARPGAttributeValue HlthAttValue = Persona->FindAttributeValueByName(TEXT("HealthPoints"));
			VitalityTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(HlthAttValue.Value));
		}
		if (IntelligenceTextBlock)
		{
			FBARPGAttributeValue IntAttValue = Persona->FindAttributeValueByName(TEXT("Intelligence"));
			IntelligenceTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(IntAttValue.Value));
		}
		if (CharismaTextBlock)
		{
			FBARPGAttributeValue ChaAttValue = Persona->FindAttributeValueByName(TEXT("Charisma"));
			CharismaTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(ChaAttValue.Value));
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