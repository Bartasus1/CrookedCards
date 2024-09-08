// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameState.h"
#include "KKPlayerState.h"

#include "Components/RoundManager.h"

#include "KrzyweKarty2/Characters/KKCharacter.h"

#include "Net/UnrealNetwork.h"

AKKGameState::AKKGameState()
{
	RoundManager = CreateDefaultSubobject<URoundManager>("Round Manager");
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
	RoundManager->MarkCharacterUsed(Character);
}

URoundManager* AKKGameState::GetRoundManager() const
{
	return RoundManager.Get();
}

void AKKGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if(HasAuthority())
	{
		if(AKKPlayerState* KKPlayerState = Cast<AKKPlayerState>(PlayerState))
		{
			RoundManager->AddPlayerState(KKPlayerState);
		}
	}
}

void AKKGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AKKGameState, GameBoard, COND_InitialOnly);
	DOREPLIFETIME(AKKGameState, Winner);
}

void AKKGameState::OnRep_Winner() const
{
	OnWinnerDecided.Broadcast(Winner);
}

