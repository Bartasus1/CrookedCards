#pragma once
#include "GameBoardStructs.generated.h"

class AKKCharacter;
class ACharacterSlot;

namespace GameBoard
{
	constexpr uint8 SizeVertical = 5;
	constexpr uint8 SizeHorizontal = 4;
}

USTRUCT(Blueprintable, BlueprintType)
struct KRZYWEKARTY2_API FBoardCoordinate
{
	GENERATED_BODY()

	FBoardCoordinate() {}
	FBoardCoordinate(int32 InRow, int32 InColumn)
	{
		if(InRow == 0 || InRow == GameBoard::SizeVertical + 1) // if board coordinate is a base slot, it's column should be 0
		{
			InColumn = 0;
		}

		Row = InRow;
		Column = InColumn;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Row = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Column = 0;
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

	ACharacterSlot* operator[](int32 Index)
	{
		return CharacterSlots[Index];
	}

	ACharacterSlot* operator[](int32 Index) const
	{
		return CharacterSlots[Index];
	}
};

USTRUCT(BlueprintType)
struct FFractionCharacters
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	AKKCharacter* BaseCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<AKKCharacter*> Characters;

	TArray<AKKCharacter*> GetAllCharacters()
	{
		return Characters += { BaseCharacter };
	}

	int32 Num() const
	{
		return Characters.Num() + 1;
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
