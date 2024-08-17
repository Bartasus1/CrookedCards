// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KrzyweKarty2/Characters/KKCharacter.h"
#include "Zakon_Rycerz.generated.h"

UCLASS()
class KRZYWEKARTY2_API AZakon_Rycerz : public AKKCharacter
{
	GENERATED_BODY()

public:
	AZakon_Rycerz();

	virtual bool Target_CanBeAttacked(AKKCharacter* Attacker, const FAttackInfo& AttackInfo) override;

protected:

	uint8 AttacksCounter = 0;
	
};
