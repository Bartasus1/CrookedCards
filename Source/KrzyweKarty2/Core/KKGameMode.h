// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KKGameMode.generated.h"

class AKKCharacter;
class AKKGameBoard;
class AKKPlayerController;
/**
 * 
 */
UCLASS()
class KRZYWEKARTY2_API AKKGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKKGameMode();

	void SetGameBoard(AKKGameBoard* NewGameBoard);

	AKKPlayerController* GetPlayer(uint8 PlayerID);
	
protected:
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	int32 GetDirectionFromID(int32 PlayerID) const;
	void SpawnFractionForPlayer(AKKPlayerController* PlayerController) const;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<AKKPlayerController*> Players;

	UPROPERTY()
	TObjectPtr<AKKGameBoard> GameBoard;
};
