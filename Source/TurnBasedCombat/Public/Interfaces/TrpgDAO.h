//// Created by Bionic Ape. All Rights Reserved.
//
//#pragma once
//
//#include "UObject/Interface.h"
//#include "TrpgDAO.generated.h"
//
//
////DECLARE_DELEGATE_OneParam(FAsyncLoadTrpgDelegate, FAsyncTrpgResponse);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncLoadTrpgDelegate, FAsyncTrpgResponse, Response);
////DECLARE_DELEGATE_OneParam(FTrpgResponseDelegate, FAsyncTrpgResponse);
////DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncSaveTrpgDelegate, FAsyncTrpgResponse, Response);
//
//DECLARE_DELEGATE_OneParam(FTrpgResponseDelegate, FAsyncTrpgResponse);
//
///**
//*
//*/
//UINTERFACE(Blueprintable)
//class TURNBASEDCOMBAT_API UTrpgDAO : public UInterface
//{
//	GENERATED_BODY()
//
//};
//
//class ITrpgDAO
//{
//	GENERATED_BODY()
//
//public:
//
//
//};
//
//#pragma region DAOOwner
//
///**
//*
//* This class needs to be implemented by the GameInstance, it would hold provide class that implements ITrpgDAO
//* 
//*/
//UINTERFACE(Blueprintable)
//class TURNBASEDCOMBAT_API UTrpgDAOOwner : public UInterface
//{
//	GENERATED_BODY()
//
//};
//
///**
//*
//* This class needs to be implemented by the GameInstance, it would hold provide class that implements ITrpgDAO
//*
//*/
//class ITrpgDAOOwner
//{
//	GENERATED_BODY()
//
//public:
//	virtual ITrpgDAO* GetTrpgDAO() const = 0;
//	virtual void SetTrpgDAO(ITrpgDAO* Dao) = 0;	
//};
//
//#pragma endregion