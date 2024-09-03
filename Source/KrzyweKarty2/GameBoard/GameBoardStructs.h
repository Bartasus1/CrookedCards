#pragma once
#include "GameBoardStructs.generated.h"

class ACharacterSlot;

namespace GameBoard
{
	const uint8 SizeVertical = 5;
	const uint8 SizeHorizontal = 4;
}

USTRUCT(Blueprintable, BlueprintType)
struct KRZYWEKARTY2_API FBoardCoordinate
{
	GENERATED_BODY()

	FBoardCoordinate() {}
	FBoardCoordinate(uint8 InRow, uint8 InColumn)
	{
		if(InRow == 0 || InRow == GameBoard::SizeVertical + 1) // if board coordinate is a base slot, it's column should be 0
		{
			InColumn = 0;
		}

		Row = InRow;
		Column = InColumn;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 Row = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 Column = 0;
};

USTRUCT(BlueprintType)
struct FCharacterSlotsRow
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<ACharacterSlot*> CharacterSlots;

	UPROPERTY(VisibleAnywhere)
	bool bIsBaseRow = false;

	void Add(ACharacterSlot* Slot)
	{
		CharacterSlots.Add(Slot);
	}

	ACharacterSlot* GetBaseSlot() const
	{
		if(bIsBaseRow)
		{
			return CharacterSlots[0];
		}
		
		return nullptr;
	}
};

UENUM(BlueprintType)
enum ESlotSelectionPolicy
{
	SSP_NoPolicy			UMETA(DisplayName="No Policy", ToolTip = "Select all slots"),
	SSP_NoCharacter			UMETA(DisplayName="No Character", ToolTip = "Select only slots which don't have a character on them"),
	SSP_WithCharacter		UMETA(DisplayName="With Character", ToolTip = "Select only slots which have a character on them"),
};

UENUM(BlueprintType)
enum class ESelectionType : uint8 // todo: think about this
{
	Blocking,
	FreeSelection,
	AdvancedFreeSelection
};
