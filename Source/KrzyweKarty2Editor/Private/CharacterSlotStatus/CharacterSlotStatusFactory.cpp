// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSlotStatus/CharacterSlotStatusFactory.h"
#include "KrzyweKarty2/GameBoard/CharacterSlotStatus/CharacterSlotStatus.h"

UCharacterSlotStatusFactory::UCharacterSlotStatusFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UCharacterSlotStatus::StaticClass();
}

UObject* UCharacterSlotStatusFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UCharacterSlotStatus>(InParent, InClass, InName, Flags);
}

FText FCharacterSlotStatusAssetTypeActions::GetName() const
{
	return FText::FromString("CharacterSlotStatus");
}

UClass* FCharacterSlotStatusAssetTypeActions::GetSupportedClass() const
{
	return UCharacterSlotStatus::StaticClass();
}

FColor FCharacterSlotStatusAssetTypeActions::GetTypeColor() const
{
	return FColor::Green;
}

uint32 FCharacterSlotStatusAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Blueprint;
}
