// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Net/UnrealNetwork.h"

#include "RoundManager.generated.h"

class AKKCharacter;
class AKKPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundChanged);


UCLASS(ClassGroup=(Custom), Within="KKGameState", BlueprintType)
class KRZYWEKARTY2_API URoundManager : public UActorComponent
{
	GENERATED_BODY()

public:
	URoundManager();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing="OnRep_RoundCounter")
	int32 RoundCounter = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	bool bIsFirstPlayerTurn = true;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnRoundChanged OnRoundChanged;

	UFUNCTION(Server, Reliable)
	void MarkCharacterUsed(AKKCharacter* Character);

	void AddPlayerState(AKKPlayerState* PlayerState);
	bool CheckIfPlayerCanMove(AKKPlayerState* PlayerState) const;

	UFUNCTION(BlueprintPure)
	AKKPlayerState* GetCurrentPlayer() const;

private:
	uint8 MaxPlayerMovePoints = 3;
	uint8 PlayerMovePoints = 0;

	UPROPERTY(Replicated)
	TArray<AKKPlayerState*> Players;
	
	TArray<AKKCharacter*> UsedCharacters;

	UFUNCTION(Server, Reliable)
	void ChangeTurn();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_RoundCounter() const;

};