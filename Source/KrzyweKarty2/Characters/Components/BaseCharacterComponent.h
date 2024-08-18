// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseCharacterComponent.generated.h"


class AKKCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class KRZYWEKARTY2_API UBaseCharacterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBaseCharacterComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	AKKCharacter* OwnerBase;

	UFUNCTION()
	void OnBaseDestroyed();
	
};
