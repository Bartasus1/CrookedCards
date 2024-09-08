// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

#include "Interfaces/OnlineSessionInterface.h"

void UKKGameInstance::Init()
{
	Super::Init();

	OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		OnlineIdentity = OnlineSubsystem->GetIdentityInterface();
	}
//
// #if WITH_EDITOR
// 	OnlineIdentity->AutoLogin(0);
// #else
// 	const FOnlineAccountCredentials AccountCredentials("AccountPortal", "", "");
// 	OnlineIdentity->Login(0, AccountCredentials);
// #endif
	
}

void UKKGameInstance::CreateSession()
{
	if(IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bAllowInvites = true;
		SessionSettings.bIsDedicated = false;
		SessionSettings.bIsLANMatch = false;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bAllowJoinViaPresence = true;
		SessionSettings.NumPublicConnections = 2;

		Session->CreateSession(0, "NewCardGame", SessionSettings);
	}
}
