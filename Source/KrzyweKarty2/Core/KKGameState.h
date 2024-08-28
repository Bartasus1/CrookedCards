// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "KKGameState.generated.h"

class URoundManager;
class AKKPlayerState;
class AKKGameBoard;
class AKKCharacter;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWinnerDecided, AKKPlayerState*, Winner);


UCLASS()
class KRZYWEKARTY2_API AKKGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AKKGameState();
	
	void RegisterCharacter(AKKCharacter* Character);

	UFUNCTION(BlueprintCallable)
	AKKGameBoard* GetGameBoard() const;

	UFUNCTION(BlueprintCallable)
	AKKCharacter* GetCharacterByUniqueID(int32 CharacterUniqueID);

	UFUNCTION(Server, Reliable)
	void MarkCharacterUsedInRound(AKKCharacter* Character);

	UFUNCTION(BlueprintCallable)
	URoundManager* GetRoundManager() const;

protected:

	virtual void AddPlayerState(APlayerState* PlayerState) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(VisibleAnywhere)
	TMap<int32, AKKCharacter*> RegisteredCharacters; // characters self-register at each own BeginPlay - that means it happens on Client as well (can't make it server-side, bcuz od TMap)

	UPROPERTY(BlueprintAssignable)
	FOnWinnerDecided OnWinnerDecided;
	
private:
	
	UPROPERTY(BlueprintReadOnly, Replicated, meta=(AllowPrivateAccess="true"))
	TObjectPtr<AKKGameBoard> GameBoard;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"), VisibleAnywhere)
	TObjectPtr<URoundManager> RoundManager;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing="OnRep_Winner", meta=(AllowPrivateAccess="true"))
	AKKPlayerState* Winner;

	UFUNCTION()
	void OnRep_Winner() const;
	
	friend class AKKGameMode;
};
