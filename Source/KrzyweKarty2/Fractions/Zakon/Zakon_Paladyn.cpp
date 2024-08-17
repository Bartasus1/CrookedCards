// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Paladyn.h"


// Sets default values
AZakon_Paladyn::AZakon_Paladyn()
{

}

bool AZakon_Paladyn::Target_CanBeAttacked(AKKCharacter* Attacker, const FAttackInfo& AttackInfo)
{
	if(GetHealth() < 15.f)
	{
		if(AttackInfo.AttackType != EAttackType::DefaultAttack)
		{
			return false;
		}
	}

	return true;
}


