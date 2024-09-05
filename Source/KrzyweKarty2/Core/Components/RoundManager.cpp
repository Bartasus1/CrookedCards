// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundManager.h"

#include "KrzyweKarty2/Characters/KKCharacter.h"
#include "KrzyweKarty2/Characters/CharacterActions/CharacterAction.h"
#include "KrzyweKarty2/Core/KKGameState.h"
#include "KrzyweKarty2/Core/KKPlayerState.h"


URoundManager::URoundManager()
{
	PrimaryComponentTick.TickInterval = 0.5;
}

void URoundManager::AddPlayerState(AKKPlayerState* PlayerState)
{
	Players.Add(PlayerState);

	if(Players.Num() >= 2)
	{
		bIsFirstPlayerTurn = false;
		ChangeTurn();
	}
}

bool URoundManager::CheckIfPlayerCanMove(AKKPlayerState* PlayerState) const
{
	TArray<bool> CanCharactersMove;
	for (const AKKCharacter* Character : PlayerState->PlayerFractionCharacters.Characters)
	{
		CanCharactersMove.Add(Character->CanCharacterBeUsed());
	}
	
	return CanCharactersMove.Contains(true);
}

void URoundManager::MarkCharacterUsed_Implementation(AKKCharacter* Character)
{
	PlayerMovePoints++;
	UsedCharacters.Add(Character);
	
	if(PlayerMovePoints >= MaxPlayerMovePoints)
	{
		ChangeTurn();
	}

	//UE_LOG(LogTemp, Warning, TEXT("%d"), bIsFirstPlayerTurn);
	if(!CheckIfPlayerCanMove(Players[!bIsFirstPlayerTurn])) // if bIsFirstPlayerTurn is true (1), then Players[bIsFirstPlayerTurn] will get us a player that is waiting, but we want the player that was waiting before 
	{
		ChangeTurn(); // change turn again if other player can't move
	}
}

void URoundManager::ChangeTurn()
{
	PlayerMovePoints = 0;
	
	RoundCounter++;
	OnRep_RoundCounter();
	
	bIsFirstPlayerTurn = !bIsFirstPlayerTurn;

	Players[0]->bIsMyTurn = bIsFirstPlayerTurn;
	Players[1]->bIsMyTurn = !bIsFirstPlayerTurn;
	
	for (AKKCharacter* Character : UsedCharacters)
	{
		if(Character != nullptr)
		{
			Character->CharacterActions = 1;
		}
	}

	UsedCharacters.Empty();
}

void URoundManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void URoundManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URoundManager, RoundCounter);
	DOREPLIFETIME(URoundManager, bIsFirstPlayerTurn);
}

void URoundManager::OnRep_RoundCounter() const
{
	OnRoundChanged.Broadcast();
}



