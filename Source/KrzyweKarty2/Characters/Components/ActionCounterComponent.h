// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionCounterComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class KRZYWEKARTY2_API UActionCounterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UActionCounterComponent();

	UPROPERTY(EditAnywhere)
	int32 CounterResetValue = 1;

	UPROPERTY(BlueprintReadOnly)
	int32 CounterValue = 0;

	UFUNCTION(BlueprintCallable, meta=(ExpandBoolAsExecs="ReturnValue"))
	bool IncrementAndCheckForReset() // returns true if the counter has reached the reset value
	{
		CounterValue++;

		if(CounterValue >= CounterResetValue)
		{
			CounterValue = 0;
			return true;
		}

		return false;
	}

};
