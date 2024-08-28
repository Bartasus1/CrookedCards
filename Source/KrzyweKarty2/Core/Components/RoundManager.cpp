// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundManager.h"

#include "Kismet/KismetSystemLibrary.h"

#include "KrzyweKarty2/Characters/KKCharacter.h"
#include "KrzyweKarty2/Core/KKPlayerState.h"


URoundManager::URoundManager()
{
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

void URoundManager::MarkCharacterUsed_Implementation(AKKCharacter* Character)
{
	PlayerMovePoints++;
	Characters.Add(Character);
	
	if(PlayerMovePoints >= MaxPlayerMovePoints)
	{
		ChangeTurn();
	}
}

void URoundManager::ChangeTurn()
{
	PlayerMovePoints = 0;
	
	bIsFirstPlayerTurn = !bIsFirstPlayerTurn;

	Players[0]->bIsMyTurn = bIsFirstPlayerTurn;
	Players[1]->bIsMyTurn = !bIsFirstPlayerTurn;

	RoundCounter++;
	UKismetSystemLibrary::PrintString(this, "Round: " + FString::FromInt(RoundCounter));
	OnRep_RoundCounter();
	
	for (AKKCharacter* Character : Characters)
	{
		if(Character != nullptr)
		{
			Character->CharacterActions = 0;
		}
	}
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



