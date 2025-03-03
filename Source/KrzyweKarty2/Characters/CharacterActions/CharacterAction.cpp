// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAction.h"
#include "KrzyweKarty2/KKBlueprintFunctionLibrary.h"
#include "KrzyweKarty2/Characters/KKCharacter.h"

bool UCharacterAction::CanExecuteAction(const AKKCharacter* Character, const AKKGameBoard* GameBoard) const
{
	if(Character->CharacterActions >= ActionWeight || (bRequireCharacterOnGameBoard && !Character->IsCharacterOnTheBoard()))
	{
		return false;
	}

	if(QueryStruct.IsValid())
	{
		const bool bNoValidSlots = UKKBlueprintFunctionLibrary::QueryCharacterSlots(Character, QueryStruct).IsEmpty();
		return !bNoValidSlots; // can't execute if there are no valid slots
	}

	return true;
}

bool UCharacterAction_Ability::CanExecuteAction(const AKKCharacter* Character, const AKKGameBoard* GameBoard) const
{
	if(!Super::CanExecuteAction(Character, GameBoard))
	{
		return false;
	}

	bool bCanAnyAbilityBeExecuted = false;
	for (uint8 i = 0; i < Character->CharacterDataAsset->ActiveAbilities.Num(); i++)
	{
		bCanAnyAbilityBeExecuted |= Character->CanActivateCharacterAbility(i);
	}

	return bCanAnyAbilityBeExecuted;
}


