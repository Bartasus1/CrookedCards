// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActionComponent.h"

#include "KrzyweKarty2/Characters/KKCharacter.h"
#include "KrzyweKarty2/Core/KKPlayerController.h"
#include "KrzyweKarty2/Core/KKPlayerState.h"


// Sets default values for this component's properties
UAbilityActionComponent::UAbilityActionComponent(): Character(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UAbilityActionComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AKKCharacter>(GetOwner());
	check(Character); // Owner should always be character
	
}

AKKPlayerController* UAbilityActionComponent::GetPlayerController() const
{
	return Cast<AKKPlayerController>(Character->PlayerState->GetPlayerController());
}

