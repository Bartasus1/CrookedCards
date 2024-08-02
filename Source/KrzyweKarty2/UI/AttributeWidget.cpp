// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"
#include "KrzyweKarty2/AbilitySystem/Attributes/KKAttributeSet.h"

void UAttributeWidget::UpdateValueOfAttribute_Implementation(const UKKAttributeSet* AttributeSet)
{
	const float AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeValueText->SetText(FText::AsNumber(AttributeValue));
	
	if(UMaterialInstanceDynamic* MaterialInstanceDynamic = AttributeImage->GetDynamicMaterial())
	{
		const int32 MaxAttributeValue = AttributeSet->GetMaxValueForAttribute(Attribute);
		const float Progress = UKismetMathLibrary::SafeDivide(AttributeValue, MaxAttributeValue);
		
		MaterialInstanceDynamic->SetScalarParameterValue("Progress", Progress);
	}
}

void UAttributeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}
