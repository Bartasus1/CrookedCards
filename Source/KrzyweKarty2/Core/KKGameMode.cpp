﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameMode.h"
#include "KKGameState.h"
#include "KKPlayerController.h"
#include "KKPlayerState.h"
#include "KrzyweKarty2/Characters/KKCharacter.h"
#include "KrzyweKarty2/GameBoard/KKGameBoard.h"


AKKGameMode::AKKGameMode()
{
	GameStateClass = AKKGameState::StaticClass();
	PlayerControllerClass = AKKPlayerController::StaticClass();
	PlayerStateClass = AKKPlayerState::StaticClass();
}

void AKKGameMode::SetGameBoard(AKKGameBoard* NewGameBoard)
{
	GameBoard = NewGameBoard;
	GetGameState<AKKGameState>()->GameBoard = GameBoard;
}

AKKPlayerController* AKKGameMode::GetPlayer(uint8 PlayerID)
{
	return Players[PlayerID];
}

APlayerController* AKKGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	const int32 PlayerNumber = Players.Num(); // 0 for first player and 1 for the second
	
	APlayerController* PlayerController =  Super::Login(NewPlayer, InRemoteRole, FString::FromInt(PlayerNumber), Options, UniqueId, ErrorMessage);
	
	if (AKKPlayerController* KKPlayerController = Cast<AKKPlayerController>(PlayerController))
	{
		KKPlayerController->PlayerID = PlayerNumber;
		Players.Add(KKPlayerController);
	}

	return PlayerController;
}

void AKKGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (const AKKPlayerController* KKPlayerController = Cast<AKKPlayerController>(NewPlayer))
	{
		int32 Number = KKPlayerController->PlayerID;
		GetWorldTimerManager().SetTimerForNextTick([this, Number]() //by the next frame, GameBoard should have it's BeginPlay called, where it assigns itself to GameMode
		{
			//UKismetSystemLibrary::PrintString(this, FString::FromInt(Number));
			SpawnFractionForPlayer(Players[Number]); // todo: maybe make it delegate based ?
		});
	}
}

int32 AKKGameMode::GetDirectionFromID(int32 PlayerID) const
{
	return (PlayerID == 0) ? 1 : -1;
}

void AKKGameMode::SpawnFractionForPlayer(AKKPlayerController* PlayerController) const
{
	AKKPlayerState* KKPlayerState = PlayerController->GetPlayerState<AKKPlayerState>();

	FFractionCharacters FractionCharacters = GameBoard->SpawnPlayerFraction(PlayerController->PlayerID, PlayerController->FractionToSpawn);
	for(AKKCharacter* Character : FractionCharacters.GetAllCharacters())
	{
		Character->Direction = GetDirectionFromID(PlayerController->PlayerID);
		Character->SetOwner(PlayerController);
		Character->SetPlayerState(KKPlayerState);
		Character->SetAutonomousProxy(true);

		Character->FinishSpawning(Character->GetTransform()); // allow calling BeginPlay on Characters
	}

	GameBoard->InitPlayerBase(PlayerController->PlayerID, FractionCharacters.BaseCharacter);
}