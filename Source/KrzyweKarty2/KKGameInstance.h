// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Interfaces/OnlineIdentityInterface.h"

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

	UFUNCTION(BlueprintCallable)
	void LoginPlayer();

	UFUNCTION(BlueprintCallable)
	void CreateSession(FString SessionPassword);

	UFUNCTION(BlueprintCallable)
	void StartSession();

protected:

	UFUNCTION(BlueprintNativeEvent)
	void OnSessionCreated(FName SessionName, bool bWasSuccessful);

private:

	bool bSessionExists = false;
	FDelegateHandle CreateSessionDelegate;
};
