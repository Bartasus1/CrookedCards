// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Interfaces/OnlineIdentityInterface.h"

#include "KKGameInstance.generated.h"

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
	void CreateSession();

protected:

	IOnlineSubsystem* OnlineSubsystem;
	IOnlineIdentityPtr OnlineIdentity;
};
