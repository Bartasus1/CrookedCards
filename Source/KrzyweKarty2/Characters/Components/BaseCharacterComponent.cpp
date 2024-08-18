// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterComponent.h"

#include "KrzyweKarty2/Characters/KKCharacter.h"
#include "KrzyweKarty2/Core/KKGameMode.h"


UBaseCharacterComponent::UBaseCharacterComponent(): OwnerBase(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UBaseCharacterComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerBase = GetOwner<AKKCharacter>();
	OwnerBase->OnCharacterDeath.AddDynamic(this, &UBaseCharacterComponent::OnBaseDestroyed);
	
}

void UBaseCharacterComponent::OnBaseDestroyed()
{
	if(AKKGameMode* GameMode = GetWorld()->GetAuthGameMode<AKKGameMode>())
	{
		GameMode->EndGame(OwnerBase->PlayerState);
	}
}

