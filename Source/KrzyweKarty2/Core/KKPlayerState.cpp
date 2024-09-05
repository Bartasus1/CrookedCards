// Fill out your copyright notice in the Description page of Project Settings.


#include "KKPlayerState.h"
#include "Net/UnrealNetwork.h"

AKKPlayerState::AKKPlayerState()
{
	
}

void AKKPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AKKPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKKPlayerState, bIsMyTurn);

	DOREPLIFETIME_CONDITION(AKKPlayerState, PlayerFractionCharacters, COND_InitialOnly);
}
