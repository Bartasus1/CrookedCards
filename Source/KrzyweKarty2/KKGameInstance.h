// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "KKGameInstance.generated.h"

class FOnlineSessionSearch;
class IOnlineSubsystem;
/**
 * 
 */
UCLASS()
class KRZYWEKARTY2_API UKKGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoginPlayer();

	UFUNCTION(BlueprintCallable)
	void CreateSession(FString SessionPassword);

	UFUNCTION(BlueprintCallable)
	void StartSession();

protected:

	UFUNCTION(BlueprintNativeEvent)
	void OnSessionCreated(FName SessionName, bool bWasSuccessful);
	
	void OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

private:

	bool bSessionExists = false;
	FDelegateHandle CreateSessionDelegate;
};
