// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "CharacterSlot.generated.h"

class UCharacterSlotStatus;
class AKKCharacter;

UCLASS()
class KRZYWEKARTY2_API ACharacterSlot : public AActor
{
	GENERATED_BODY()

public:
	ACharacterSlot();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> SlotMeshComponent;
	
	// MANAGING CHARACTER //
	void AssignCharacterToSlot(AKKCharacter* Character);

	UFUNCTION()
	void RemoveCharacterFromSlot();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool HasCharacterAtSlot() const;

	UFUNCTION(BlueprintCallable)
	AKKCharacter* GetCharacterAtSlot() const;

	// SLOT LOCAL STATUS //
	UFUNCTION(BlueprintCallable)
	void SetLocalStatus(UCharacterSlotStatus* NewLocalStatus);

	UFUNCTION(BlueprintPure)
	const FGameplayTag& GetLocalStatusTag() const;

protected:
	// MOVEMENT //
	UPROPERTY(BlueprintReadOnly)
	bool bExecuteImmediateMovement = false;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void MoveCharacter(AKKCharacter* Character);
	
public:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Replicated, VisibleAnywhere)
	int32 CharacterInSlotUniqueID = 0; // 0 - no character, any other number - character at the slot | greater than 0 - first player, smaller than 0 - second player

	UPROPERTY(BlueprintReadOnly, Replicated, VisibleAnywhere)
	int32 CharacterSlotID = 0; // slot's own ID, set at the BeginPlay

	
protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UCharacterSlotStatus> LocalStatus;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstanceDynamic;

	friend class AKKGameBoard;
	
};
