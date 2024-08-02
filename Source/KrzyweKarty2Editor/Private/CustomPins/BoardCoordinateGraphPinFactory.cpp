// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPins/BoardCoordinateGraphPinFactory.h"
#include "KismetPins/SGraphPinVector2D.h"
#include "KrzyweKarty2/GameBoard/GameBoardStructs.h"

TSharedPtr<SGraphPin> FBoardCoordinateGraphPinFactory::CreatePin(UEdGraphPin* Pin) const
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	/*
		Check if pin is struct, and then check if that pin is of struct type we want customize
	*/
	if (Pin->PinType.PinCategory == K2Schema->PC_Struct && Pin->PinType.PinSubCategoryObject == FBoardCoordinate::StaticStruct()) 
	{
		return SNew(SGraphPinVector2D<float>, Pin); //and return our customized pin widget ;).
	}
	
	return nullptr;
}
