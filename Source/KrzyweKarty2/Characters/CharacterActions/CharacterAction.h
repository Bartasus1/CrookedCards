// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"

#include "KrzyweKarty2/GameBoard/GameBoardQueries/GameBoardQuery.h"

#include "UObject/Object.h"
#include "CharacterAction.generated.h"

class AKKGameBoard;
class AKKCharacter;
/**
 * 
 */
UCLASS(Abstract, EditInlineNew, DefaultToInstanced)
class KRZYWEKARTY2_API UCharacterAction : public UObject
{
	GENERATED_BODY()

public:

	UCharacterAction()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 ActionWeight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRequireCharacterOnGameBoard = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BaseStruct = "GameBoardQuery"))
	FInstancedStruct QueryStruct;

	virtual bool CanExecuteAction(const AKKCharacter* Character, const AKKGameBoard* GameBoard) const;
};

UCLASS()
class UCharacterAction_Summon : public UCharacterAction
{
	GENERATED_BODY()

public:

	UCharacterAction_Summon()
	{
		ActionWeight = 0;
		bRequireCharacterOnGameBoard = false;
		QueryStruct = FInstancedStruct::Make(FGameBoardQuery_Summon());
	}
};

UCLASS()
class UCharacterAction_Movement : public UCharacterAction
{
	GENERATED_BODY()

public:

	UCharacterAction_Movement()
	{
		ActionWeight = 1;
		bRequireCharacterOnGameBoard = true;
		QueryStruct = FInstancedStruct::Make(FGameBoardQuery_Movement());
	}
};

UCLASS()
class UCharacterAction_Attack : public UCharacterAction
{
	GENERATED_BODY()

public:

	UCharacterAction_Attack()
	{
		ActionWeight = 2;
		bRequireCharacterOnGameBoard = true;
		QueryStruct = FInstancedStruct::Make(FGameBoardQuery_Attack());
	}
};

UCLASS()
class UCharacterAction_Ability : public UCharacterAction
{
	GENERATED_BODY()

public:

	UCharacterAction_Ability()
	{
		ActionWeight = 2;
		bRequireCharacterOnGameBoard = true;
		// don't initialize QueryStruct to make it invalid
	}

	virtual bool CanExecuteAction(const AKKCharacter* Character, const AKKGameBoard* GameBoard) const override; // checks if ANY ability can be executed
};
