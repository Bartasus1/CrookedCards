// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KrzyweKarty2/Characters/KKCharacter.h"
#include "Zakon_Pikinier.generated.h"

UCLASS()
class KRZYWEKARTY2_API AZakon_Pikinier : public AKKCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AZakon_Pikinier();

	virtual void Attacker_OnAttackBegin(AKKCharacter* TargetCharacter, const FAttackInfo& AttackInfo, FGameplayEffectSpec& Spec) override;

protected:

	uint8 ExtendedAttackDamage = 6;
};
