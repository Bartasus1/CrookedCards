// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameBoard.h"
#include "CharacterSlot.h"
#include "Fraction/Fraction.h"

#include "Kismet/KismetSystemLibrary.h"

#include "KrzyweKarty2/Characters/KKCharacter.h"
#include "KrzyweKarty2/Core/KKGameMode.h"
#include "Net/UnrealNetwork.h"

AKKGameBoard::AKKGameBoard()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	CharacterSlotClass = ACharacterSlot::StaticClass();
	BaseIndexes.Init(0, 2);
}

ACharacterSlot* AKKGameBoard::GetCharacterSlotByID(uint8 SlotID, ESlotSelectionPolicy SelectionPolicy) const
{
	return GetCharacterSlotByCoordinates(GetBoardCoordinateByID(SlotID), SelectionPolicy);
}

ACharacterSlot* AKKGameBoard::GetCharacterSlotByRelativeDirection(uint8 SourceSlotID, FRelativeDirection RelativeDirection, ESlotSelectionPolicy SelectionPolicy, bool bIncludeBaseSlots) const
{
	const FBoardCoordinate SourceCoordinates = GetBoardCoordinateByID(SourceSlotID);
	const FBoardCoordinate TargetCoordinates(SourceCoordinates.Row + RelativeDirection.VerticalDirection, SourceCoordinates.Column + RelativeDirection.HorizontalDirection);
	
	if(!AreCoordinatesValid(TargetCoordinates))
	{
		return nullptr;
	}
	
	if(GameBoard[TargetCoordinates.Row].bIsBaseRow) 
	{
		if(!IsBaseInRange(SourceCoordinates) || !bIncludeBaseSlots) // return null if target is base but we are at wrong coords or we aren't searching for the base
		{
			return nullptr;
		}
	}

	return GetCharacterSlotByCoordinates(TargetCoordinates, SelectionPolicy);
}

ACharacterSlot* AKKGameBoard::GetCharacterSlotByCoordinates(FBoardCoordinate SlotCoordinates, ESlotSelectionPolicy SelectionPolicy) const
{
	const uint8 X = SlotCoordinates.Row;
	const uint8 Y = SlotCoordinates.Column;
	
	if(AreCoordinatesValid(X, Y))
	{
		ACharacterSlot* CharacterSlot = GameBoard[X].CharacterSlots[Y];
		
		switch (SelectionPolicy)
		{
		case SSP_NoPolicy:
			return CharacterSlot;
		case SSP_NoCharacter:
			if(!CharacterSlot->HasCharacterAtSlot())
			{
				return CharacterSlot;
			}
			break;
		case SSP_WithCharacter:
			if(CharacterSlot->HasCharacterAtSlot())
			{
				return CharacterSlot;
			}
			break;
		default:
			break;
		}
	}
	
	return nullptr;
}

FBoardCoordinate AKKGameBoard::GetBoardCoordinateByID(uint8 SlotID) const 
{
	if(SlotID == 0 || SlotID == SlotCounter - 1)
	{
		const uint8 BaseSlotRow = (SlotID == 0) ? 0 : GameBoard.Num() - 1;
		
		return FBoardCoordinate(BaseSlotRow, 0); // base slots
	}
	
	const uint8 RowIndex = 1 + (SlotID - 1) / GameBoard::SizeHorizontal; // 4 -> 1; 8 -> 2
	const uint8 ColumnIndex = (SlotID - 1) - (RowIndex - 1) * GameBoard::SizeHorizontal; // 4 -> 3; 8 -> 3
	
	return FBoardCoordinate(RowIndex, ColumnIndex);
}

uint8 AKKGameBoard::GetTotalMapSize() const
{
	return SlotCounter;
}

void AKKGameBoard::AddCharacterToBoard(AKKCharacter* Character, ACharacterSlot* CharacterSlot)
{
	if(Character && CharacterSlot)
	{
		if(CharacterSlot->HasCharacterAtSlot())
		{
			return;
		}

		CharacterSlot->bExecuteImmediateMovement = true; // for summoning, disable animations
		CharacterSlot->AssignCharacterToSlot(Character);
		CharacterSlot->bExecuteImmediateMovement = false; // from now on, animate character movements
	}
}

void AKKGameBoard::MoveCharacterToSlot(AKKCharacter* Character, ACharacterSlot* CharacterSlot)
{
	if(Character && CharacterSlot)
	{
		if(CharacterSlot->HasCharacterAtSlot())
		{
			return;
		}
		
		if(ACharacterSlot* CurrentCharacterSlot = GetCharacterSlotByID(Character->CharacterSlotID))
		{
			CurrentCharacterSlot->RemoveCharacterFromSlot();
			CharacterSlot->AssignCharacterToSlot(Character);
		}
	}
}

uint8 AKKGameBoard::GetDistanceBetweenSlots(uint8 SlotA, uint8 SlotB)
{
	const FBoardCoordinate CoordinateA = GetBoardCoordinateByID(SlotA);
	const FBoardCoordinate CoordinateB = GetBoardCoordinateByID(SlotB);

	if(IsBaseInRange(CoordinateA) || IsBaseInRange(CoordinateB))
	{
		if(BaseIndexes.Contains(SlotA) || BaseIndexes.Contains(SlotB))
		{
			return 1;
		}
	}

	const FVector2D VectorA = FVector2D(CoordinateA.Row, CoordinateA.Column);
	const FVector2D VectorB = FVector2D(CoordinateB.Row, CoordinateB.Column);
	
	return FVector2D::Distance(VectorA, VectorB);
}

void AKKGameBoard::BeginPlay()
{
	Super::BeginPlay();

	if(AKKGameMode* GameMode = GetWorld()->GetAuthGameMode<AKKGameMode>()) // works only on the server
	{
		CreateGameBoard();
		
		GameMode->SetGameBoard(this);
	}
}

void AKKGameBoard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AKKGameBoard, GameBoard, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AKKGameBoard, SlotCounter, COND_InitialOnly);
}

ACharacterSlot* AKKGameBoard::CreateSlotActor(const FVector& SlotLocation, const FRotator& SlotRotation, bool bExecuteImmediateMovement /* = true */)
{
	ACharacterSlot* CharacterSlot = GetWorld()->SpawnActor<ACharacterSlot>(CharacterSlotClass, SlotLocation, SlotRotation);
	CharacterSlot->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	CharacterSlot->CharacterSlotID = SlotCounter++;
	CharacterSlot->bExecuteImmediateMovement = bExecuteImmediateMovement;

	return CharacterSlot;
}

void AKKGameBoard::CreateBaseSlot(uint8 BaseIndex)
{
	checkf(BaseIndex < 2, TEXT("Bad input for BaseIndex (outside of expected 0-1 range)"));
	
	const float BaseLocationX[2] = { -340.f, 340.f}; //consider 302.f
	const float BaseRotationYaw[2] = { -90.f, 90.f};
	
	FCharacterSlotsRow BaseSlotRow;
	BaseSlotRow.bIsBaseRow = true;

	const FVector BaseSlotLocation = GetActorLocation() + FVector(BaseLocationX[BaseIndex], 0.f, 0.f);
	const FRotator BaseSlotRotation = FRotator(0.f, BaseRotationYaw[BaseIndex], 0.f);

	ACharacterSlot* BaseCharacterSlot = CreateSlotActor(BaseSlotLocation, BaseSlotRotation);
	BaseSlotRow.Add(BaseCharacterSlot);
	
	GameBoard.Add(BaseSlotRow);
	BaseIndexes[BaseIndex] = BaseCharacterSlot->CharacterSlotID;
	
}

void AKKGameBoard::CreateGameBoard()
{
	const FVector StartLocation = GetActorLocation() + FVector(-250.f, -150.f, 1.f);
	const uint8 SpacingVertical = 125;
	const uint8 SpacingHorizontal = 100;
	
	CreateBaseSlot(0);
	
	for (uint8 i = 0; i < GameBoard::SizeVertical; i++)
	{
		FCharacterSlotsRow SlotsRow;
		
		for (uint8 j = 0; j < GameBoard::SizeHorizontal; j++)
		{
			FVector SlotLocation = StartLocation;
			SlotLocation.X += i * SpacingVertical;
			SlotLocation.Y += j * SpacingHorizontal;
			
			ACharacterSlot* CharacterSlot = CreateSlotActor(SlotLocation, GetActorRotation(), false);
			SlotsRow.Add(CharacterSlot);
		}

		GameBoard.Add(SlotsRow);
	}
	
	CreateBaseSlot(1);
}

ACharacterSlot* AKKGameBoard::GetBaseSlotByPlayerID(uint8 BaseIndex)
{
	switch (BaseIndex)
	{
	case  0:
		return GameBoard[0].GetBaseSlot();
	case 1:
		{
			const int32 LastRowIndex = GameBoard.Num() - 1;
			return GameBoard[LastRowIndex].GetBaseSlot();
		}
	default:
		break;
	} 
	return nullptr;
}

TArray<ACharacterSlot*> AKKGameBoard::GetCharacterSlotsArray() const
{
	TArray<ACharacterSlot*> CharacterSlots;
	for (const auto& SlotsRow : GameBoard)
	{
		CharacterSlots.Append(SlotsRow.CharacterSlots);
	}

	return CharacterSlots;
}

FFractionCharacters AKKGameBoard::SpawnPlayerFraction(int32 PlayerID, TSubclassOf<AFraction> FractionClass)
{
	if(HasAuthority() && FractionClass)
	{
		AFraction* Fraction = GetWorld()->SpawnActor<AFraction>(FractionClass, GetTransformForFraction(PlayerID));

		FFractionCharacters FractionCharacters = Fraction->SpawnFractionCharacters();
		
		return FractionCharacters;
	}
	
	return FFractionCharacters();
}

void AKKGameBoard::InitPlayerBase_Implementation(int32 PlayerID, AKKCharacter* BaseCharacter)
{
	if(ACharacterSlot* BaseSlot = GetBaseSlotByPlayerID(PlayerID))
	{
		BaseSlot->AssignCharacterToSlot(BaseCharacter);
	}
}

bool AKKGameBoard::AreCoordinatesValid(const uint8 X, const uint8 Y) const
{
	return GameBoard.IsValidIndex(X) && GameBoard[X].CharacterSlots.IsValidIndex(Y);
}

bool AKKGameBoard::AreCoordinatesValid(FBoardCoordinate BoardCoordinate) const
{
	return AreCoordinatesValid(BoardCoordinate.Row, BoardCoordinate.Column);
}

bool AKKGameBoard::IsBaseInRange(FBoardCoordinate SourceCoordinate) const
{
	if(SourceCoordinate.Row == 1 || SourceCoordinate.Row == GameBoard::SizeVertical) // edge rows of the board
	{
		return SourceCoordinate.Column == 1 || SourceCoordinate.Column == 2; // center columns of the board
	}

	return false;
}

uint8 AKKGameBoard::GetClosestBaseSlotID(uint8 SourceID) const
{
	uint8 BaseSlotIDs[2] = {0, 21};
	for (const uint8 BaseSlotID : BaseSlotIDs)
	{
		const int8 DistanceToBase = SourceID - BaseSlotID;
		if(FMath::Abs(DistanceToBase) <= (GameBoard::SizeHorizontal + 1))// discard distance greater than one row + 1 (for safety)
		{
			return BaseSlotID;
		}
	}

	return -1;
}
