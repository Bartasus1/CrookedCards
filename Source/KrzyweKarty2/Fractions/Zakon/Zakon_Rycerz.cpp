// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Rycerz.h"


// Sets default values
AZakon_Rycerz::AZakon_Rycerz()
{
}

bool AZakon_Rycerz::Target_CanBeAttacked(AKKCharacter* Attacker, const FAttackInfo& AttackInfo)
{
	AttacksCounter++;

	if(AttacksCounter >= 3)
	{
		AttacksCounter = 0;
		return false;
	}

	return true;
}


