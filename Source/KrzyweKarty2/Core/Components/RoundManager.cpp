// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundManager.h"

#include "KrzyweKarty2/Characters/KKCharacter.h"
#include "KrzyweKarty2/Core/KKPlayerState.h"


URoundManager::URoundManager()
{

	SetIsReplicatedByDefault(true);
}


bool URoundManager::CheckIfPlayerCanMove(AKKPlayerState* PlayerState) const
{
	TArray<bool> CanCharactersMove;
	for (const AKKCharacter* Character : PlayerState->GetPlayableCharacters())
	{
		if(Character)
		{
			CanCharactersMove.Add(Character->CanCharacterBeUsed());
		}
	}
	
	return CanCharactersMove.Contains(true);
}

AKKPlayerState* URoundManager::GetCurrentPlayer() const
{
	return Players[!bIsFirstPlayerTurn];
}

void URoundManager::AddPlayerState(AKKPlayerState* PlayerState)
{
	Players.Add(PlayerState);
	if(Players.Num() >= 2)
	{
		bIsFirstPlayerTurn = false; // flip so that first player starts
		ChangeTurn();
	}
}

void URoundManager::MarkCharacterUsed_Implementation(AKKCharacter* Character)
{
	PlayerMovePoints++;
	UsedCharacters.Add(Character);
	
	if(PlayerMovePoints >= MaxPlayerMovePoints)
	{
		ChangeTurn();
	}
	
	if(!CheckIfPlayerCanMove(GetCurrentPlayer())) // if new player can't move roll back to the previous player - we might want to change that
	{
		ChangeTurn(); // change turn again if other player can't move
	}
}

void URoundManager::ChangeTurn_Implementation()
{
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

	PlayerMovePoints = 0;
	
	RoundCounter++;
	OnRep_RoundCounter();
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
	DOREPLIFETIME(URoundManager, Players);
}

void URoundManager::OnRep_RoundCounter() const
{
	OnRoundChanged.Broadcast();
}



