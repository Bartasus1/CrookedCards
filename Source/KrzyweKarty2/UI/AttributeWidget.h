// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KrzyweKarty2/AbilitySystem/Attributes/KKAttributeSet.h"
#include "AttributeWidget.generated.h"


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttribute Attribute;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> AttributeImage;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> AttributeValueText;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateValueOfAttribute(const UKKAttributeSet* AttributeSet);

	virtual void NativeConstruct() override;
	
};
