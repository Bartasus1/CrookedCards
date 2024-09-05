// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"

#include "GameFramework/Actor.h"

#include "KrzyweKarty2/GameBoard/GameBoardStructs.h"

#include "Fraction.generated.h"

class AKKCharacter;
class UArrowComponent;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class USpawnIndicator : public UBoxComponent
{
	GENERATED_BODY()

public:

	USpawnIndicator();
};

UCLASS()
class KRZYWEKARTY2_API AFraction : public AActor
{
	GENERATED_BODY()

public:
	AFraction();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Fraction Data")
	FText FractionName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cards")
	TArray<TSubclassOf<AKKCharacter>> CharactersToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cards")
	TSubclassOf<AKKCharacter> BaseClass;

	FFractionCharacters SpawnFractionCharacters();
	
protected:
	TArray<AKKCharacter*> SpawnCharacters();
	AKKCharacter* SpawnBase() const;

private:
	
#if WITH_EDITOR
	UFUNCTION()
	void SpawnComponents();
	
	void RemoveComponents();
#endif
	
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
};
