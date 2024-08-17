// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Pikinier.h"

#include "KrzyweKarty2/GameBoard/KKGameBoard.h"


// Sets default values
AZakon_Pikinier::AZakon_Pikinier()
{

}

void AZakon_Pikinier::Attacker_OnAttackBegin(AKKCharacter* TargetCharacter, const FAttackInfo& AttackInfo, FGameplayEffectSpec& Spec)
{
	const uint8 TargetSlotID = TargetCharacter->CharacterSlotID;

	float Strength = AttributeSet->GetStrength();
	switch (GetGameBoard()->GetDistanceBetweenSlots(CharacterSlotID, TargetSlotID))
	{
	case 2:
		Strength = ExtendedAttackDamage;
		break;
	default:
		break;
	}

	Spec.SetByCallerNameMagnitudes.FindOrAdd("Strength", Strength);
}


