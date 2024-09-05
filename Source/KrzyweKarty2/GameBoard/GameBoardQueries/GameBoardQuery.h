// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KrzyweKarty2/GameBoard/GameBoardStructs.h"
#include "GameBoardQuery.generated.h"

struct FRelativeDirection;
class AKKCharacter;
class AKKGameBoard;
class ACharacterSlot;
/**
 * 
 */
USTRUCT(BlueprintType)
struct KRZYWEKARTY2_API FGameBoardQuery
{
	GENERATED_BODY()
	
	virtual TArray<ACharacterSlot*> ExecuteQuery(const AKKGameBoard* GameBoard, const AKKCharacter* Character) const
	{
		return TArray<ACharacterSlot*>();
	}

	virtual ~FGameBoardQuery()
	{
	}
};

USTRUCT(BlueprintType)
struct FGameBoardQuery_Normal : public FGameBoardQuery // Returns all slots that match the RelativeDirections
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRelativeDirection> RelativeDirections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESlotSelectionPolicy> SelectionPolicy;

	virtual TArray<ACharacterSlot*> ExecuteQuery(const AKKGameBoard* GameBoard, const AKKCharacter* Character) const override;
};

USTRUCT(BlueprintType)
struct FGameBoardQuery_Projectile : public FGameBoardQuery_Normal // Returns slots that are first in given direction, matching Selection Policy
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESlotSelectionPolicy> BlockingSelectionPolicy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBreakIfNoSlotFound = true;

	virtual TArray<ACharacterSlot*> ExecuteQuery(const AKKGameBoard* GameBoard, const AKKCharacter* Character) const override;
	
};

USTRUCT(BlueprintType)
struct FGameBoardQuery_Attack : public FGameBoardQuery
{
	GENERATED_BODY()
	
	virtual TArray<ACharacterSlot*> ExecuteQuery(const AKKGameBoard* GameBoard, const AKKCharacter* Character) const override;

protected:
	TUniquePtr<FGameBoardQuery> GetGameBoardQueryForAttack(const AKKCharacter* Character) const;
};

USTRUCT(BlueprintType)
struct FGameBoardQuery_Movement : public FGameBoardQuery
{
	GENERATED_BODY()
	
	virtual TArray<ACharacterSlot*> ExecuteQuery(const AKKGameBoard* GameBoard, const AKKCharacter* Character) const override;
};

USTRUCT(BlueprintType)
struct FGameBoardQuery_Summon : public FGameBoardQuery
{
	GENERATED_BODY()

	virtual TArray<ACharacterSlot*> ExecuteQuery(const AKKGameBoard* GameBoard, const AKKCharacter* Character) const override;
};