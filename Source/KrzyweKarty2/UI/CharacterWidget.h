// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterWidget.generated.h"

class AKKCharacter;
class UAttributeWidget;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class KRZYWEKARTY2_API UCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> CharacterImage;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
    TObjectPtr<UTextBlock> CharacterNameText;

	// ATTRIBUTE WIDGETS //
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UAttributeWidget> HealthAttributeWidget;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UAttributeWidget> ManaAttributeWidget;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UAttributeWidget> DefenceAttributeWidget;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UAttributeWidget> StrengthAttributeWidget;

	// FUNCTIONS //
	UFUNCTION(BlueprintCallable)
	void SetCharacter(AKKCharacter* InCharacter);

	UFUNCTION(BlueprintCallable)
	void UpdateAllAttributeWidgets() const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AKKCharacter> Character;
	
};
