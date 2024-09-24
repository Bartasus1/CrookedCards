// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "FindAndJoinOnlineSession.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFindAndJoinOnlineSessionDelegate);

UCLASS()
class KRZYWEKARTY2_API UFindAndJoinOnlineSession : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Online", WorldContext = "WorldContextObject"))
	static UFindAndJoinOnlineSession* FindOnlineSession(UObject* WorldContextObject, FString SessionPassword);

	UPROPERTY(BlueprintAssignable)
	FFindAndJoinOnlineSessionDelegate OnSessionFound;

	UPROPERTY(BlueprintAssignable)
	FFindAndJoinOnlineSessionDelegate OnSessionNotFound;
	
protected:
	FString SessionPassword;

	void FindSessions(FName InSearchKey, const FString& SearchValue);
	void JoinSession(const FOnlineSessionSearchResult& SessionToJoin);
};
