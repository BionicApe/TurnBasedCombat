// Created by Bionic Ape. All Rights Reserved.


#include "Components/TrpgControlComponent.h"
#include "Interfaces/BAMultiplayerDAO.h"
#include "Interfaces/BionicApeHUDInterface.h"
#include "GameFramework/PlayerController.h"
#include "FighterProfile.h"
#include "Mockup/MockupFocusable.h"

#define LOCTEXT_NAMESPACE "TrpgControlComponent"

void UTrpgControlComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTrpgControlComponent, Profiles);
	DOREPLIFETIME(UTrpgControlComponent, MainProfile);
}


bool UTrpgControlComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	WroteSomething |= Channel->ReplicateSubobjectList(Profiles, *Bunch, *RepFlags);
	WroteSomething |= Channel->ReplicateSubobject(MainProfile, *Bunch, *RepFlags);
	return WroteSomething;
}

void UTrpgControlComponent::AddProfile(UFighterProfile* NewFighterProfile, bool bIsMainInventory)
{
	if (NewFighterProfile)
	{
		Profiles.Add(NewFighterProfile);
	}

	if (bIsMainInventory)
	{
		MainProfile = NewFighterProfile;
	}
}

void UTrpgControlComponent::AddXP(UFighterProfile* FighterProfile, AMockupFocusable* MockupFocusable)
{
	Server_AddXP(FighterProfile, MockupFocusable);
}

void UTrpgControlComponent::Server_AddXP_Implementation(UFighterProfile* FighterProfile, AMockupFocusable* MockupFocusable)
{

#pragma region ValidationCheck

	if (!FighterProfile)
	{
		const FText ErrorText = LOCTEXT("FighterProfileNull", "The Fighter Profile is null");
		UE_LOG(LogTemp, Error, TEXT("UTrpgControlComponent::Server_AddXP_Implementation: %s"), *ErrorText.ToString());
		Client_Notify(false, ErrorText.ToString());
		return;
	}

	if (!Profiles.Contains(FighterProfile))
	{
		const FText ErrorText = FText::Format(LOCTEXT("TrpgNotCointained", "The Trpg with {0} does not belong to this TrpgControlComponent"), FText::FromString(FighterProfile->Id));
		UE_LOG(LogTemp, Error, TEXT("UTrpgControlComponent::Server_AddXP_Implementation: %s"), *ErrorText.ToString());
		Client_Notify(false, ErrorText.ToString());
		return;
	}

	if (!MockupFocusable)
	{
		const FText ErrorText = LOCTEXT("NoMockupFocusable", "MockupFocusable is null");
		UE_LOG(LogTemp, Error, TEXT("UTrpgControlComponent::Server_AddXP_Implementation: %s"));
		Client_Notify(false, ErrorText.ToString());
		return;
	}

	IBAMultiplayerDAOOwner* BAMultiplayerDAOOwner = Cast<IBAMultiplayerDAOOwner>(GetWorld()->GetGameInstance());
	if (!BAMultiplayerDAOOwner)
	{
		const FText ErrorText = LOCTEXT("BAMultiplayerDAOOwnerFailed", "The GameInstance doesn't implement IBAMultiplayerDAOOwner");
		UE_LOG(LogTemp, Error, TEXT("UTrpgControlComponent::Server_AddXP_Implementation: %s"), *ErrorText.ToString());
		Client_Notify(false, ErrorText.ToString());
		return;
	}

	IBAMultiplayerDAO* Dao = BAMultiplayerDAOOwner->GetBAMultiplayerDAO();
	if (!Dao)
	{
		const FText ErrorText = LOCTEXT("BAMultiplayerDAOFailed", "BAMultiplayerDAOOwner->GetBAMultiplayerDAO() is null");
		UE_LOG(LogTemp, Error, TEXT("UTrpgControlComponent::Server_AddXP_Implementation: %s"), *ErrorText.ToString());
		Client_Notify(false, ErrorText.ToString());
		return;
	}

#pragma endregion


	FighterProfile->AddXp(MockupFocusable->AmountToAdd);
	FBAProfileResponseDelegate Delegate;
	Delegate.BindUObject(this, &UTrpgControlComponent::OnUpdateProfile);
	Dao->Update(FighterProfile, Delegate);
}

void UTrpgControlComponent::OnUpdateProfile(FBAProfileResponse Response)
{
	Client_Notify(Response.bIsSuccessful, Response.ErrorMessage);
}

void UTrpgControlComponent::AddAttributePoints(UFighterProfile* FighterProfile, AMockupFocusable* MockupFocusable)
{
	//Server_AddAttributePoints(FighterProfile, MockupFocusable);
}

void UTrpgControlComponent::Server_AddAttributePoints_Implementation(UFighterProfile* FighterProfile, AMockupFocusable* MockupFocusable)
{

#pragma region ValidationCheck

	if (!FighterProfile)
	{
		const FText ErrorText = LOCTEXT("FighterProfileNull", "The Fighter Profile is null");
		UE_LOG(LogTemp, Error, TEXT("UTrpgControlComponent::Server_AddAttributePoints_Implementation: %s"), *ErrorText.ToString());
		Client_Notify(false, ErrorText.ToString());
		return;
	}

	if (!Profiles.Contains(FighterProfile))
	{
		const FText ErrorText = FText::Format(LOCTEXT("TrpgNotCointained", "The Trpg with {0} does not belong to this TrpgControlComponent"), FText::FromString(FighterProfile->Id));
		UE_LOG(LogTemp, Error, TEXT("UTrpgControlComponent::Server_AddAttributePoints_Implementation: %s"), *ErrorText.ToString());
		Client_Notify(false, ErrorText.ToString());
		return;
	}

	if (!MockupFocusable)
	{
		const FText ErrorText = LOCTEXT("NoMockupFocusable", "MockupFocusable is null");
		UE_LOG(LogTemp, Error, TEXT("UTrpgControlComponent::Server_AddAttributePoints_Implementation: %s"));
		Client_Notify(false, ErrorText.ToString());
		return;
	}

	IBAMultiplayerDAOOwner* BAMultiplayerDAOOwner = Cast<IBAMultiplayerDAOOwner>(GetWorld()->GetGameInstance());
	if (!BAMultiplayerDAOOwner)
	{
		const FText ErrorText = LOCTEXT("BAMultiplayerDAOOwnerFailed", "The GameInstance doesn't implement IBAMultiplayerDAOOwner");
		UE_LOG(LogTemp, Error, TEXT("UTrpgControlComponent::Server_AddAttributePoints_Implementation: %s"), *ErrorText.ToString());
		Client_Notify(false, ErrorText.ToString());
		return;
	}

	IBAMultiplayerDAO* Dao = BAMultiplayerDAOOwner->GetBAMultiplayerDAO();
	if (!Dao)
	{
		const FText ErrorText = LOCTEXT("BAMultiplayerDAOFailed", "BAMultiplayerDAOOwner->GetBAMultiplayerDAO() is null");
		UE_LOG(LogTemp, Error, TEXT("UTrpgControlComponent::Server_AddAttributePoints_Implementation: %s"), *ErrorText.ToString());
		Client_Notify(false, ErrorText.ToString());
		return;
	}

#pragma endregion

	FighterProfile->AddAttributePoints(1);
	FBAProfileResponseDelegate Delegate;
	Delegate.BindUObject(this, &UTrpgControlComponent::OnUpdateProfile);
	Dao->Update(FighterProfile, Delegate);
}

//void UTrpgControlComponent::AddAttributePoint(UFighterProfile* FighterProfile, const FString& AttributeName)
//{
//	Server_AddAttributePoint(FighterProfile, AttributeName);
//}

//void UTrpgControlComponent::Server_AddAttributePoint_Implementation(UFighterProfile* FighterProfile, const FString& AttributeName)
//{
//
//#pragma region ValidationCheck
//
//	if (!FighterProfile)
//	{
//		const FText ErrorText = LOCTEXT("FighterProfileNull", "The Fighter Profile is null");
//		UE_LOG(LogTemp, Error, TEXT("UTrpgControlComponent::Server_AddAttributePoint_Implementation: %s"), *ErrorText.ToString());
//		Client_Notify(false, ErrorText.ToString());
//		return;
//	}
//
//	if (!Profiles.Contains(FighterProfile))
//	{
//		const FText ErrorText = FText::Format(LOCTEXT("TrpgNotCointained", "The Trpg with {0} does not belong to this TrpgControlComponent"), FText::FromString(FighterProfile->Id));
//		UE_LOG(LogTemp, Error, TEXT("UTrpgControlComponent::Server_AddAttributePoint_Implementation: %s"), *ErrorText.ToString());
//		Client_Notify(false, ErrorText.ToString());
//		return;
//	}
//
//	IBAMultiplayerDAOOwner* BAMultiplayerDAOOwner = Cast<IBAMultiplayerDAOOwner>(GetWorld()->GetGameInstance());
//	if (!BAMultiplayerDAOOwner)
//	{
//		const FText ErrorText = LOCTEXT("BAMultiplayerDAOOwnerFailed", "The GameInstance doesn't implement IBAMultiplayerDAOOwner");
//		UE_LOG(LogTemp, Error, TEXT("UTrpgControlComponent::Server_AddAttributePoint_Implementation: %s"), *ErrorText.ToString());
//		Client_Notify(false, ErrorText.ToString());
//		return;
//	}
//
//	IBAMultiplayerDAO* Dao = BAMultiplayerDAOOwner->GetBAMultiplayerDAO();
//	if (!Dao)
//	{
//		const FText ErrorText = LOCTEXT("BAMultiplayerDAOFailed", "BAMultiplayerDAOOwner->GetBAMultiplayerDAO() is null");
//		UE_LOG(LogTemp, Error, TEXT("UTrpgControlComponent::Server_AddAttributePoint_Implementation: %s"), *ErrorText.ToString());
//		Client_Notify(false, ErrorText.ToString());
//		return;
//	}
//
//#pragma endregion
//
//	bool const bIsSuccesful = FighterProfile->AddAttributePoint(AttributeName);
//
//	if (bIsSuccesful)
//	{
//		FBAProfileResponseDelegate Delegate;
//		Delegate.BindUObject(this, &UTrpgControlComponent::OnUpdateProfile);
//		Dao->Update(FighterProfile, Delegate);
//	}
//	else
//	{
//		const FText ErrorText = LOCTEXT("BAMultiplayerDAOFailed", "Attribute Point not added");
//		UE_LOG(LogTemp, Error, TEXT("UTrpgControlComponent::Server_AddAttributePoint_Implementation: %s"), *ErrorText.ToString());
//		Client_Notify(false, ErrorText.ToString());
//	}
//}

#undef LOCTEXT_NAMESPACE // "TrpgControlComponent"