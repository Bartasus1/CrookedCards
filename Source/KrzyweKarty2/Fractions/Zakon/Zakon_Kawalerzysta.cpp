// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Kawalerzysta.h"


// Sets default values
AZakon_Kawalerzysta::AZakon_Kawalerzysta()
{

}

TArray<FRelativeDirection> AZakon_Kawalerzysta::GetDirectionsForMovement() const
{
	TArray<FRelativeDirection> MovementDirections = Super::GetDirectionsForMovement();
	MovementDirections.Add({Direction * 2, 0});

	return MovementDirections;
}


