// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoardQuery.h"

#include "KrzyweKarty2/Characters/CharacterStructs.h"
#include "KrzyweKarty2/Characters/KKCharacter.h"
#include "KrzyweKarty2/GameBoard/CharacterSlot.h"
#include "KrzyweKarty2/GameBoard/KKGameBoard.h"


TArray<ACharacterSlot*> FGameBoardQuery_Normal::ExecuteQuery(const AKKGameBoard* GameBoard, AKKCharacter* Character) const
{
	if(RelativeDirections.IsEmpty())
	{
		return Super::ExecuteQuery(GameBoard, Character);
	}

	TArray<ACharacterSlot*> CharacterSlots;
	for (const FRelativeDirection& Direction : RelativeDirections)
	{
		if(ACharacterSlot* CharacterSlot = GameBoard->GetCharacterSlotByRelativeDirection(Character->CharacterSlotID, Direction, SelectionPolicy))
		{
			CharacterSlots.Add(CharacterSlot);
		}
	}

	return CharacterSlots;
}

TArray<ACharacterSlot*> FGameBoardQuery_Projectile::ExecuteQuery(const AKKGameBoard* GameBoard, AKKCharacter* Character) const
{
	if(RelativeDirections.IsEmpty())
	{
		return Super::ExecuteQuery(GameBoard, Character);
	}
	
	TArray<ACharacterSlot*> CharacterSlots;

	TArray<TArray<FRelativeDirection>> SortedDirections;
	FRelativeDirection::SortDirections(RelativeDirections).GenerateValueArray(SortedDirections);
	
	for (const TArray<FRelativeDirection>& Directions : SortedDirections)
	{
		for (const FRelativeDirection& Direction : Directions)
		{
			//UE_LOG(LogTemp, Warning, TEXT("%d %d"), Direction.VerticalDirection, Direction.HorizontalDirection);
			if(ACharacterSlot* CharacterSlot = GameBoard->GetCharacterSlotByRelativeDirection(Character->CharacterSlotID, Direction, SelectionPolicy))
			{
				CharacterSlots.Add(CharacterSlot);

				if(GameBoard->GetCharacterSlotByRelativeDirection(Character->CharacterSlotID, Direction, BlockingSelectionPolicy))
				{
					break;
				}
			}
			else
			{
				if(bBreakIfNoSlotFound)
				{
					break; // there's no slot in given direction or slot doesn't meet SelectionPolicy, so we can also break
				}
			}
		}
	}

	return CharacterSlots;
}

TArray<ACharacterSlot*> FGameBoardQuery_Attack::ExecuteQuery(const AKKGameBoard* GameBoard, AKKCharacter* Character) const
{
	const TUniquePtr<FGameBoardQuery> GameBoardQuery = GetGameBoardQueryForAttack(Character);
	
	TArray<ACharacterSlot*> CharacterSlots = GameBoardQuery->ExecuteQuery(GameBoard, Character);
	
	if(ACharacterSlot* BaseSlot = GameBoard->GetBaseSlotInRange(Character->CharacterSlotID))
	{
		CharacterSlots.Add(BaseSlot);
	}
	
	return CharacterSlots; 
	
}

TUniquePtr<FGameBoardQuery> FGameBoardQuery_Attack::GetGameBoardQueryForAttack(AKKCharacter* Character) const
{
	if(!Character->IsRangedCharacter())
	{
		TUniquePtr<FGameBoardQuery_Normal> GameBoardQuery = MakeUnique<FGameBoardQuery_Normal>();

		GameBoardQuery->RelativeDirections = Character->GetDirectionsForDefaultAttack();
		GameBoardQuery->SelectionPolicy = SSP_WithCharacter;

		return GameBoardQuery;
	}
	else
	{
		TUniquePtr<FGameBoardQuery_Projectile> GameBoardQuery = MakeUnique<FGameBoardQuery_Projectile>();

		GameBoardQuery->RelativeDirections = Character->GetDirectionsForDefaultAttack();
		GameBoardQuery->SelectionPolicy = SSP_WithCharacter;
		GameBoardQuery->BlockingSelectionPolicy = SSP_WithCharacter;
		GameBoardQuery->bBreakIfNoSlotFound = false;

		return GameBoardQuery;
	}
}

TArray<ACharacterSlot*> FGameBoardQuery_Movement::ExecuteQuery(const AKKGameBoard* GameBoard, AKKCharacter* Character) const
{
	const TUniquePtr<FGameBoardQuery_Projectile> GameBoardQuery = MakeUnique<FGameBoardQuery_Projectile>();

	GameBoardQuery->RelativeDirections = Character->GetDirectionsForMovement();
	GameBoardQuery->SelectionPolicy = SSP_NoCharacter;
	GameBoardQuery->BlockingSelectionPolicy = SSP_WithCharacter;

	return GameBoardQuery->ExecuteQuery(GameBoard, Character);
}
