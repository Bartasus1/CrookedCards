// Fill out your copyright notice in the Description page of Project Settings.


#include "FindAndJoinOnlineSession.h"

#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"

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
	Search->QuerySettings.Set(InSearchKey, SearchValue, EOnlineComparisonOp::Equals); 

	
	Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateLambda([this, Search](bool bWasSuccessful)
	{
		const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
		const IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
	 
		if (bWasSuccessful)
		{
			UE_LOG(LogTemp, Log, TEXT("Found sessions..."));
		 
			for (const FOnlineSessionSearchResult& SessionInSearchResult : Search->SearchResults)
			{
				UE_LOG(LogTemp, Warning, TEXT("Found session with id: %s"), *SessionInSearchResult.GetSessionIdStr());

				FString ConnectString;
				//Ensure the connection string is resolvable and store the info in ConnectInfo and in SessionToJoin
				if (Session->GetResolvedConnectString(SessionInSearchResult, NAME_GamePort, ConnectString))
				{
					JoinSession(SessionInSearchResult);

					OnSessionFound.Broadcast();
				}
		 
				// For the tutorial we will join the first session found automatically. Usually you would loop through all the sessions and determine which one is best to join. 
				break;            
			}
		}
		else
		{
			OnSessionNotFound.Broadcast();
		}
	}
	));
	    
	UE_LOG(LogTemp, Log, TEXT("Finding session."));
	 
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
