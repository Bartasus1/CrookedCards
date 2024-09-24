// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"

void UKKGameInstance::Init()
{
	Super::Init();

	const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	const IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(this, &UKKGameInstance::OnSessionJoined));
}

void UKKGameInstance::LoginPlayer()
{
	const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	const IOnlineIdentityPtr OnlineIdentity = Subsystem->GetIdentityInterface();

	const FOnlineAccountCredentials AccountCredentials("AccountPortal", "", "");
	OnlineIdentity->Login(0, AccountCredentials);
	
	// if(GetWorld()->WorldType == EWorldType::PIE)
	// {
	// 	
	// }
	// else
	// {
	// 	if(!OnlineIdentity->AutoLogin(0))
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Couldn't auto login"));
	// 	
	// 		const FOnlineAccountCredentials AccountCredentials("AccountPortal", "", "");
	// 		OnlineIdentity->Login(0, AccountCredentials);
	// 	}
	// }
}

void UKKGameInstance::CreateSession(FString SessionPassword)
{
	const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	const IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	CreateSessionDelegate = Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(this, &UKKGameInstance::OnSessionCreated));
	
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowInvites = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinInProgress = false;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.Settings.Add("Password", FOnlineSessionSetting(SessionPassword, EOnlineDataAdvertisementType::ViaOnlineService));

	if(!Session->CreateSession(0, "Crooked Cards Session", SessionSettings))
	{
		Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegate);
		CreateSessionDelegate.Reset();

		if (bSessionExists)
		{
			Session->DestroySession("Crooked Cards Session");
		}
	}
}

void UKKGameInstance::StartSession()
{
	const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	const IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	Session->StartSession("Crooked Cards Session");
}

void UKKGameInstance::OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(Result == EOnJoinSessionCompleteResult::Success)
	{
		const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
		const IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
		
		FString TravelURL;
		Session->GetResolvedConnectString(SessionName, TravelURL);

		GetFirstLocalPlayerController(GetWorld())->ClientTravel(TravelURL, TRAVEL_Absolute);
	}
}

void UKKGameInstance::OnSessionCreated_Implementation(FName SessionName, bool bWasSuccessful)
{
	const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	const IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	if (bWasSuccessful)
	{
		bSessionExists = true;
		UE_LOG(LogTemp, Log, TEXT("Session: %s Created!"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create session!"));
	}

	Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegate);
	CreateSessionDelegate.Reset();
}
