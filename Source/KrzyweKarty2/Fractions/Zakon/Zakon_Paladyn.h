// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KrzyweKarty2/Characters/KKCharacter.h"
#include "Zakon_Paladyn.generated.h"

UCLASS()
class KRZYWEKARTY2_API AZakon_Paladyn : public AKKCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AZakon_Paladyn();


	virtual bool Target_CanBeAttacked(AKKCharacter* Attacker, const FAttackInfo& AttackInfo) override;
};
