// Created by Bionic Ape. All Rights Reserved.


#include "UI/ProfileWidget.h"
#include "PlayerCombatPawn.h"
#include "Fight.h"
#include "Combatant.h"
#include "BAProfile.h"
#include "Fight.h"
#include "TrpgCombatTypes.h"

#include "Components/TextBlock.h"

#include "Components/TrpgControlComponent.h"

#include "Kismet/KismetTextLibrary.h"
#include "TurnBasedCombatLib.h"


bool UProfileWidget::Initialize()
{
	bool bResult = Super::Initialize();
	if (bResult)
	{
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
		}
		Profile = NewProfile;//We set the new one!
		if (Profile)
		{
			//add new delegates
			Profile->OnAttributesChanged.AddDynamic(this, &UProfileWidget::OnAttributesChanged);
		}
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
		if (XpTextBlock)
		{
			XpTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(Profile->Attributes.Xp));
		}
		if (CreationTimeTextBlock)
		{
			CreationTimeTextBlock->SetText(UKismetTextLibrary::AsDateTime_DateTime(Profile->CreationTime));
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
		if (FelonyTextBlock)
		{
			FelonyTextBlock->SetText(UEnum::GetDisplayValueAsText(Profile->Attributes.Felony));
		}
		if (XPLevelTextBlock)
		{
			XPLevelTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(Profile->Attributes.XPLevel));
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
	}
}

void UProfileWidget::AddAttributePoint(FString AttributeName)
{
	UE_LOG(LogTemp, Log, TEXT("UInventoryItemEntryListEntryStoreW::OnButtonPressed"));

	//if (ConfirmWidgetClass && StoreDH && StoreDH->SellerItemEntry && StoreDH->SellerItemEntry->Item)
	//{
	//	if (UConfirmWidget* ConfirmWidget = CreateWidget<UConfirmWidget>(GetWorld(), ConfirmWidgetClass))
	//	{
	//		ConfirmWidget->SetBodyText(
	//			FText::Format(LOCTEXT("ConfirmBuy", "Do you want to buy {0} for {1} coins?"),	
	//				FText::FromName(StoreDH->SellerItemEntry->Item->Name),
	//				StoreDH->SellerItemEntry->Price));
	//		ConfirmWidget->OnFinishConfirmWidget.AddDynamic(this, &UProfileWidget::OnFinishConfirmWidget, AttributeName);
	//		ConfirmWidget->AddToViewport();
	//	}
	//}
}

void UProfileWidget::OnAttributesChanged()
{
	Refresh();
}

void UProfileWidget::OnFinishConfirmWidget(bool bIsConfirmed, FString AttributeName)
{
	//if (bIsConfirmed)
	//{
	//	UTrpgControlComponent* TrpgControlComp = UTurnBasedCombatLib::GetTrpgControlComp(GetOwningPlayer());
	//	TrpgControlComp->IncreaseAttribute(AttributeName);
	//}
}
