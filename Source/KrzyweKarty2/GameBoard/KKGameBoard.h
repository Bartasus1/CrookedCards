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
	ACharacterSlot* GetCharacterSlotByID(uint8 SlotID, ESlotSelectionPolicy SelectionPolicy = SSP_NoPolicy) const;

	UFUNCTION(BlueprintPure = false)
	ACharacterSlot* GetCharacterSlotByRelativeDirection(uint8 SourceSlotID, FRelativeDirection RelativeDirection, ESlotSelectionPolicy SelectionPolicy = SSP_NoPolicy, bool bIncludeBaseSlots = false) const;

	UFUNCTION(BlueprintPure = false)
	ACharacterSlot* GetCharacterSlotByCoordinates(FBoardCoordinate SlotCoordinates, ESlotSelectionPolicy SelectionPolicy = SSP_NoPolicy) const;

	UFUNCTION(BlueprintPure)
	uint8 GetTotalMapSize() const;

	UFUNCTION(BlueprintCallable)
	void AddCharacterToBoard(AKKCharacter* Character, ACharacterSlot* CharacterSlot);

	UFUNCTION(BlueprintCallable)
	void MoveCharacterToSlot(AKKCharacter* Character, ACharacterSlot* CharacterSlot);
	
protected:

	FBoardCoordinate GetBoardCoordinateByID(uint8 SlotID) const;

public:
	UFUNCTION(BlueprintNativeEvent)
	void InitPlayerBase(int32 PlayerID, AKKCharacter* BaseCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	void InitPlayableCharacter(AKKCharacter* Character) const;
	
	UFUNCTION(BlueprintImplementableEvent)
	const FTransform GetTransformForFraction(int32 PlayerID);

	FFractionCharacters SpawnPlayerFraction(int32 PlayerID, TSubclassOf<AFraction> FractionClass);

private:
	bool AreCoordinatesValid(uint8 X, uint8 Y) const;
	bool AreCoordinatesValid(FBoardCoordinate BoardCoordinate) const;
	bool IsBaseInRange(FBoardCoordinate SourceCoordinate) const;

	uint8 GetClosestBaseSlotID(uint8 SourceID) const; // use only when sourceID is in the first row from the base (1-4 and 17-20)

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	ACharacterSlot* CreateSlotActor(const FVector& SlotLocation, const FRotator& SlotRotation, bool bExecuteImmediateMovement = true);
	void CreateBaseSlot(uint8 BaseIndex);
	void CreateGameBoard();

	void CreateConnections();
	
	ACharacterSlot* GetBaseSlotByPlayerID(uint8 BaseIndex);
private:
	const uint8 SizeVertical = 5;
	const uint8 SizeHorizontal = 4;

	UPROPERTY(Replicated)
	uint8 SlotCounter = 0;

	UPROPERTY(VisibleAnywhere, Replicated)
	TArray<FCharacterSlotsRow> GameBoard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	TSubclassOf<ACharacterSlot> CharacterSlotClass;

public:
	UFUNCTION(BlueprintCallable)
	TArray<ACharacterSlot*> GetCharacterSlotsArray() const;
	
};
