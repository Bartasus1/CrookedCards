// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterWidget.h"
#include "AttributeWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "KrzyweKarty2/Characters/CharacterDataAsset.h"
#include "KrzyweKarty2/Characters/KKCharacter.h"

void UCharacterWidget::SetCharacter(AKKCharacter* InCharacter)
{
	if(IsValid(InCharacter))
	{
		Character = InCharacter;

		CharacterImage->SetBrushFromSoftTexture(Character->GetCharacterDataAsset()->CharacterCardTexture);
		CharacterNameText->SetText(Character->GetCharacterDataAsset()->CharacterName);

		HealthAttributeWidget->InitializeAttributeWidget(InCharacter);
		ManaAttributeWidget->InitializeAttributeWidget(InCharacter);
		DefenceAttributeWidget->InitializeAttributeWidget(InCharacter);
		StrengthAttributeWidget->InitializeAttributeWidget(InCharacter);

		OnCharacterSet();
	}
}
