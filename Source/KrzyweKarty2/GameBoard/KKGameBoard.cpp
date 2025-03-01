// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameBoard.h"
#include "CharacterSlot.h"
#include "Fraction/Fraction.h"

#include "Kismet/KismetSystemLibrary.h"

#include "KrzyweKarty2/Characters/KKCharacter.h"
#include "KrzyweKarty2/Core/KKGameMode.h"
#include "Net/UnrealNetwork.h"

#define PrintString(message) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, message)

AKKGameBoard::AKKGameBoard()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	CharacterSlotClass = ACharacterSlot::StaticClass();
	BaseIndexes.Init(0, 2);
}

ACharacterSlot* AKKGameBoard::GetCharacterSlotByID(int32 SlotID, ESlotSelectionPolicy SelectionPolicy) const
{
	return GetCharacterSlotByCoordinates(GetBoardCoordinateByID(SlotID), SelectionPolicy);
}

ACharacterSlot* AKKGameBoard::GetCharacterSlotByRelativeDirection(int32 SourceSlotID, FRelativeDirection RelativeDirection, ESlotSelectionPolicy SelectionPolicy) const
{
	const FBoardCoordinate SourceCoordinates = GetBoardCoordinateByID(SourceSlotID);
	const FBoardCoordinate TargetCoordinates(SourceCoordinates.Row + RelativeDirection.VerticalDirection, SourceCoordinates.Column + RelativeDirection.HorizontalDirection);

	if(AreCoordinatesValid(TargetCoordinates) && GameBoard[TargetCoordinates.Row].bIsBaseRow) // there's GetBaseSlotInRange if you want to get the base slot
	{
		return nullptr;
	}
	
	return GetCharacterSlotByCoordinates(TargetCoordinates, SelectionPolicy);
}

ACharacterSlot* AKKGameBoard::GetCharacterSlotByCoordinates(FBoardCoordinate SlotCoordinates, ESlotSelectionPolicy SelectionPolicy) const
{
	if(ACharacterSlot* CharacterSlot = GetCharacterSlotAtCoordinates(SlotCoordinates))
	{
		
		if(CharacterSlot->MatchesSelectionPolicy(SelectionPolicy))
		{
			return CharacterSlot;
		}
	}
	
	return nullptr;
}

FBoardCoordinate AKKGameBoard::GetBoardCoordinateByID(int32 SlotID) const 
{
	if(SlotID == 0 || SlotID == TotalMapSize - 1)
	{
		const int32 BaseSlotRow = (SlotID == 0) ? 0 : GameBoard.Num() - 1;
		
		return FBoardCoordinate(BaseSlotRow, 0); // base slots
	}
	
	const int32 RowIndex = 1 + (SlotID - 1) / GameBoard::SizeHorizontal; // 4 -> 1; 8 -> 2
	const int32 ColumnIndex = (SlotID - 1) - (RowIndex - 1) * GameBoard::SizeHorizontal; // 4 -> 3; 8 -> 3
	
	return FBoardCoordinate(RowIndex, ColumnIndex);
}

int32 AKKGameBoard::GetTotalMapSize() const
{
	return TotalMapSize;
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

int32 AKKGameBoard::GetDistanceBetweenSlots(int32 SlotA, int32 SlotB)
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
	DOREPLIFETIME(AKKGameBoard, TotalMapSize);
}

ACharacterSlot* AKKGameBoard::CreateSlotActor(const FVector& SlotLocation, const FRotator& SlotRotation, bool bExecuteImmediateMovement /* = true */)
{
	ACharacterSlot* CharacterSlot = GetWorld()->SpawnActor<ACharacterSlot>(CharacterSlotClass, SlotLocation, SlotRotation);
	CharacterSlot->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	CharacterSlot->CharacterSlotID = TotalMapSize++;
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
	const int32 SpacingVertical = 125;
	const int32 SpacingHorizontal = 100;
	
	CreateBaseSlot(0);
	
	for (int32 i = 0; i < GameBoard::SizeVertical; i++)
	{
		FCharacterSlotsRow SlotsRow;
		
		for (int32 j = 0; j < GameBoard::SizeHorizontal; j++)
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

ACharacterSlot* AKKGameBoard::GetBaseSlotByBaseIndex(int32 BaseIndex) const
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
	UE_LOG(LogTemp, Warning, TEXT("Inside spawning function!!"));
	if(HasAuthority() && FractionClass)
	{
		AFraction* Fraction = GetWorld()->SpawnActor<AFraction>(FractionClass, GetTransformForFraction(PlayerID));

		FFractionCharacters FractionCharacters = Fraction->SpawnFractionCharacters();
		
		UE_LOG(LogTemp, Warning, TEXT("Spawning completed!! Spawned characters: %d"), FractionCharacters.Num());
		
		return FractionCharacters;
	}
	
	return FFractionCharacters();
}

ACharacterSlot* AKKGameBoard::GetCharacterSlotAtCoordinates(FBoardCoordinate BoardCoordinate) const
{
	if(AreCoordinatesValid(BoardCoordinate))
	{
		return GameBoard[BoardCoordinate.Row][BoardCoordinate.Column];
	}

	return nullptr;
}

void AKKGameBoard::InitPlayerBase_Implementation(int32 PlayerID, AKKCharacter* BaseCharacter)
{
	if(ACharacterSlot* BaseSlot = GetBaseSlotByBaseIndex(PlayerID))
	{
		BaseSlot->AssignCharacterToSlot(BaseCharacter);
	}
}

bool AKKGameBoard::AreCoordinatesValid(const int32 X, const int32 Y) const
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

ACharacterSlot* AKKGameBoard::GetBaseSlotInRange(int32 SourceSlotID) const
{
	const FBoardCoordinate BoardCoordinate = GetBoardCoordinateByID(SourceSlotID);
	return GetBaseSlotInRange(BoardCoordinate);
}

ACharacterSlot* AKKGameBoard::GetBaseSlotInRange(FBoardCoordinate SourceCoordinate) const
{
	if(!IsBaseInRange(SourceCoordinate))
	{
		return nullptr;
	}

	const uint8 BaseIndex = (SourceCoordinate.Row == 1) ? 0 : 1;
	return GetBaseSlotByBaseIndex(BaseIndex);
}
