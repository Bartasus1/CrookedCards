// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KrzyweKarty2/Characters/KKCharacter.h"
#include "Zakon_Kawalerzysta.generated.h"

UCLASS()
class KRZYWEKARTY2_API AZakon_Kawalerzysta : public AKKCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AZakon_Kawalerzysta();

	virtual TArray<FRelativeDirection> GetDirectionsForMovement() const override;
};
