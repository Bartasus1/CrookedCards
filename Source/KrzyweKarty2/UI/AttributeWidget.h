// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KrzyweKarty2/AbilitySystem/Attributes/KKAttributeSet.h"
#include "AttributeWidget.generated.h"


class AKKCharacter;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class KRZYWEKARTY2_API UAttributeWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attribute")
	FGameplayAttribute Attribute;

	void InitializeAttributeWidget(const AKKCharacter* Character);
	
protected:
	void OnAttributeValueChange(const FOnAttributeChangeData& Data, const int32 MaxAttributeValue);

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnAttributeValueChange")
	void BP_OnAttributeValueChange(const float NewValue, const float OldValue, const int32 MaxAttributeValue);
};
