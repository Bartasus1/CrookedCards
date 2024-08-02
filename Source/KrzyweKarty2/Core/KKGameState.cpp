// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameState.h"

#include "KrzyweKarty2/Characters/KKCharacter.h"

#include "Net/UnrealNetwork.h"

AKKGameState::AKKGameState()
{

}

AKKGameBoard* AKKGameState::GetGameBoard() const
{
	return GameBoard;
}

void AKKGameState::RegisterCharacter(AKKCharacter* Character)
{
	RegisteredCharacters.Add(Character->GetCharacterUniqueID(), Character);
}

AKKCharacter* AKKGameState::GetCharacterByUniqueID(int32 CharacterUniqueID)
{
	if(AKKCharacter** CharacterPtr = RegisteredCharacters.Find(CharacterUniqueID))
	{
		return *CharacterPtr;
	}

	return nullptr;
}

void AKKGameState::MarkCharacterUsedInRound_Implementation(AKKCharacter* Character)
{
	CharactersUsedInRound.Add(Character);
}

void AKKGameState::ChangeTurn_Implementation()
{
	bFirstPlayerTurn = !bFirstPlayerTurn;

	for(AKKCharacter* Character : CharactersUsedInRound)
	{
		if(Character != nullptr)
		{
			Character->CharacterActions = 0;
		}
	}
}

void AKKGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AKKGameState, GameBoard, COND_InitialOnly);
}

