// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "KKGameState.generated.h"

class AKKGameBoard;
class AKKCharacter;
/**
 * 
 */


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
	
	UFUNCTION(Server, Reliable)
	void ChangeTurn();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(VisibleAnywhere)
	TMap<int32, AKKCharacter*> RegisteredCharacters; // characters self-register at each own BeginPlay - that means it happens on Client as well

	UPROPERTY(VisibleAnywhere, Replicated)
	TArray<AKKCharacter*> CharactersUsedInRound;

	UPROPERTY(VisibleAnywhere, Replicated)
	bool bFirstPlayerTurn = true;
	
private:
	
	UPROPERTY(BlueprintReadOnly, Replicated, meta=(AllowPrivateAccess="true"))
	TObjectPtr<AKKGameBoard> GameBoard;

	friend class AKKGameMode;
};
