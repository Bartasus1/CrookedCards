// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeWidget.h"
#include "KrzyweKarty2/AbilitySystem/Attributes/KKAttributeSet.h"
#include "KrzyweKarty2/Characters/KKCharacter.h"


void UAttributeWidget::InitializeAttributeWidget(const AKKCharacter* Character)
{
	UAbilitySystemComponent* AbilitySystemComponent = Character->GetAbilitySystemComponent();
	UKKAttributeSet* AttributeSet = Character->GetAttributeSet();
	
	const float AttributeValue = Attribute.GetNumericValue(AttributeSet);
	const int32 MaxAttributeValue = AttributeSet->GetMaxValueForAttribute(Attribute);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UAttributeWidget::OnAttributeValueChange, MaxAttributeValue);
	
	BP_OnAttributeValueChange(AttributeValue, AttributeValue, MaxAttributeValue); // call to initialize everything
}

void UAttributeWidget::OnAttributeValueChange(const FOnAttributeChangeData& Data, const int32 MaxAttributeValue)
{
	BP_OnAttributeValueChange(Data.NewValue, Data.OldValue, MaxAttributeValue);
}
