// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameBoardStructs.h"

#include "Fraction/Fraction.h"

#include "GameFramework/Actor.h"
#include "KKGameBoard.generated.h"

class AFraction;
struct FRelativeDirection;
class USpawnIndicator;
class AKKCharacter;

UCLASS()
class KRZYWEKARTY2_API AKKGameBoard : public AActor
{
	GENERATED_BODY()

public:
	AKKGameBoard();

	UFUNCTION(BlueprintPure = false)
	ACharacterSlot* GetCharacterSlotByID(int32 SlotID, ESlotSelectionPolicy SelectionPolicy = SSP_NoPolicy) const;

	UFUNCTION(BlueprintPure = false)
	ACharacterSlot* GetCharacterSlotByRelativeDirection(int32 SourceSlotID, FRelativeDirection RelativeDirection, ESlotSelectionPolicy SelectionPolicy = SSP_NoPolicy) const;

	UFUNCTION(BlueprintPure = false)
	ACharacterSlot* GetCharacterSlotByCoordinates(FBoardCoordinate SlotCoordinates, ESlotSelectionPolicy SelectionPolicy = SSP_NoPolicy) const;

	UFUNCTION(BlueprintPure)
	int32 GetTotalMapSize() const;

	UFUNCTION(BlueprintCallable)
	void AddCharacterToBoard(AKKCharacter* Character, ACharacterSlot* CharacterSlot);

	UFUNCTION(BlueprintCallable)
	void MoveCharacterToSlot(AKKCharacter* Character, ACharacterSlot* CharacterSlot);

	UFUNCTION(BlueprintCallable)
	int32 GetDistanceBetweenSlots(int32 SlotA, int32 SlotB);
	
protected:

	FBoardCoordinate GetBoardCoordinateByID(int32 SlotID) const;

public:
	UFUNCTION(BlueprintNativeEvent)
	void InitPlayerBase(int32 PlayerID, AKKCharacter* BaseCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	void InitPlayableCharacter(AKKCharacter* Character) const;
	
	UFUNCTION(BlueprintImplementableEvent)
	const FTransform GetTransformForFraction(int32 PlayerID);

	FFractionCharacters SpawnPlayerFraction(int32 PlayerID, TSubclassOf<AFraction> FractionClass);

private:
	ACharacterSlot* GetCharacterSlotAtCoordinates(FBoardCoordinate BoardCoordinate) const;
	bool AreCoordinatesValid(int32 X, int32 Y) const;
	bool AreCoordinatesValid(FBoardCoordinate BoardCoordinate) const;
	bool IsBaseInRange(FBoardCoordinate SourceCoordinate) const;

public:
	ACharacterSlot* GetBaseSlotInRange(int32 SourceSlotID) const;
	ACharacterSlot* GetBaseSlotInRange(FBoardCoordinate SourceCoordinate) const;

	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	ACharacterSlot* CreateSlotActor(const FVector& SlotLocation, const FRotator& SlotRotation, bool bExecuteImmediateMovement = true);
	void CreateBaseSlot(uint8 BaseIndex);
	void CreateGameBoard();
	
	ACharacterSlot* GetBaseSlotByBaseIndex(int32 BaseIndex) const;
private:

	UPROPERTY(Replicated)
	int32 TotalMapSize = 0;

	UPROPERTY(VisibleAnywhere, Replicated)
	TArray<FCharacterSlotsRow> GameBoard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	TSubclassOf<ACharacterSlot> CharacterSlotClass;

	TArray<int32> BaseIndexes; 

public:
	UFUNCTION(BlueprintCallable)
	TArray<ACharacterSlot*> GetCharacterSlotsArray() const;
	
};
