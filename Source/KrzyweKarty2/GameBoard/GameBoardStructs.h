#pragma once
#include "GameBoardStructs.generated.h"

class ACharacterSlot;

USTRUCT(Blueprintable, BlueprintType)
struct KRZYWEKARTY2_API FBoardCoordinate
{
	GENERATED_BODY()

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

	ACharacterSlot* GetBaseSlot()
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