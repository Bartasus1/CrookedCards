// Fill out your copyright notice in the Description page of Project Settings.


#include "FindAndJoinOnlineSession.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Online/OnlineSessionNames.h"

void UFindAndJoinOnlineSession::Activate()
{
	Super::Activate();

	FindSessions("Password", SessionPassword);
}

UFindAndJoinOnlineSession* UFindAndJoinOnlineSession::FindOnlineSession(UObject* WorldContextObject, FString SessionPassword)
{
	UFindAndJoinOnlineSession* Action = NewObject<UFindAndJoinOnlineSession>();
	Action->SessionPassword = SessionPassword;
	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}

void UFindAndJoinOnlineSession::FindSessions(const FName InSearchKey, const FString& SearchValue)
{
	const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	const IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
	
	TSharedRef<FOnlineSessionSearch> Search = MakeShared<FOnlineSessionSearch>();
	Search->bIsLanQuery = false;
	Search->MaxSearchResults = 20;
	Search->QuerySettings.Set(InSearchKey, SearchValue, EOnlineComparisonOp::Equals);
	Search->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);

	
	Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateLambda([this](bool bWasSuccessful, const TSharedRef<FOnlineSessionSearch>& SessionSearch)
	{
		const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
		const IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
	 
		if (bWasSuccessful && !SessionSearch->SearchResults.IsEmpty())
		{
			for (const FOnlineSessionSearchResult& SessionInSearchResult : SessionSearch->SearchResults)
			{
				UE_LOG(LogTemp, Warning, TEXT("Found session with id: %s"), *SessionInSearchResult.GetSessionIdStr());

				FString ConnectString;
				if (Session->GetResolvedConnectString(SessionInSearchResult, NAME_GamePort, ConnectString))
				{
					UE_LOG(LogTemp, Log, TEXT("Joining session."));
					if (!Session->JoinSession(0, "Crooked Cards Session", SessionInSearchResult))
					{
						UE_LOG(LogTemp, Warning, TEXT("Join session failed"));
					}
					
					OnSessionFound.Broadcast();
					break;
				}
			}
		}
		else
		{
			OnSessionNotFound.Broadcast();
		}
	}, Search
	));
	    
	UE_LOG(LogTemp, Log, TEXT("Finding sessions...."));
	 
	if (!Session->FindSessions(0, Search))
	{
		UE_LOG(LogTemp, Warning, TEXT("Find session failed"));
	}
}

void UFindAndJoinOnlineSession::JoinSession(const FOnlineSessionSearchResult& SessionToJoin)
{
	const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	const IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	UE_LOG(LogTemp, Log, TEXT("Joining session."));
	if (!Session->JoinSession(0, "Crooked Cards Session", SessionToJoin))
	{
		UE_LOG(LogTemp, Warning, TEXT("Join session failed"));
	}
}
