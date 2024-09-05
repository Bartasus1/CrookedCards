// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InstancedStruct.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "KKBlueprintFunctionLibrary.generated.h"

struct FQueryTest;
class UGameBoardQuery;
struct FRelativeDirection;
class AKKCharacter;
class AKKGameBoard;
class ACharacterSlot;
/**
 * 
 */
UCLASS()
class KRZYWEKARTY2_API UKKBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	static FText GetFractionNameByTag(const FGameplayTag& FractionTag);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static AKKGameBoard* GetGameBoard(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable)
	static TArray<ACharacterSlot*> QueryCharacterSlots(const AKKCharacter* Character, const FInstancedStruct& Query);
};
