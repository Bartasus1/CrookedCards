// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

#include "Factories/Factory.h"
#include "CharacterSlotStatusFactory.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY2EDITOR_API UCharacterSlotStatusFactory : public UFactory
{
	GENERATED_BODY()
public:
	UCharacterSlotStatusFactory();
	
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};


class  FCharacterSlotStatusAssetTypeActions : public FAssetTypeActions_Base
{

public:
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;
};