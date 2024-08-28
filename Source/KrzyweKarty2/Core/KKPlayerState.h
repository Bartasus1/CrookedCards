// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "KKPlayerState.generated.h"

class AKKCharacter;
/**
 * 
 */
UCLASS()
class KRZYWEKARTY2_API AKKPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AKKPlayerState();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	bool bIsUsingAbility = false;

	UPROPERTY(BlueprintReadOnly, Replicated, VisibleAnywhere)
	bool bIsMyTurn = false;

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
