// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityActionComponent.generated.h"


class AKKPlayerController;
class AKKCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAbilityFinishedDelegate);

UCLASS(ClassGroup=(AbilityActions), meta=(BlueprintSpawnableComponent))
class KRZYWEKARTY2_API UAbilityActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityActionComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	AKKCharacter* Character;

	UFUNCTION(BlueprintCallable)
	AKKPlayerController* GetPlayerController() const;
	
};
