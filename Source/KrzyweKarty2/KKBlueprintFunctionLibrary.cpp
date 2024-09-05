// Fill out your copyright notice in the Description page of Project Settings.


#include "KKBlueprintFunctionLibrary.h"
#include "KKCharacterDeveloperSettings.h"

#include "Characters/KKCharacter.h"

#include "Core/KKGameState.h"

#include "GameBoard/GameBoardQueries/GameBoardQuery.h"

FText UKKBlueprintFunctionLibrary::GetFractionNameByTag(const FGameplayTag& FractionTag)
{
	const UKKCharacterDeveloperSettings* DeveloperSettings = UKKCharacterDeveloperSettings::Get();

	return DeveloperSettings->FractionTagToNameMap.FindRef(FractionTag);
}

AKKGameBoard* UKKBlueprintFunctionLibrary::GetGameBoard(const UObject* WorldContextObject)
{
	const AKKGameState* GameState = WorldContextObject->GetWorld()->GetGameState<AKKGameState>();
	return GameState->GetGameBoard();
}


TArray<ACharacterSlot*> UKKBlueprintFunctionLibrary::QueryCharacterSlots(const AKKCharacter* Character, const FInstancedStruct& Query)
{
	const AKKGameBoard* GameBoard = GetGameBoard(Character);
	const FGameBoardQuery* GameBoardQuery = Query.GetPtr<FGameBoardQuery>();

	check(GameBoardQuery);
	return GameBoardQuery->ExecuteQuery(GameBoard, Character);
}
