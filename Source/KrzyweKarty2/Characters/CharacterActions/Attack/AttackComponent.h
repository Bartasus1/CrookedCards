// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

class UAttackSequence;

UENUM()
enum class EAttackStage : uint8
{
	PreAttack	UMETA(DisplayName = "Pre-Attack", ToolTip="Pre eliminary attack stage. Can be used to stop it"),
	Before		UMETA(DisplayName = "Before", ToolTip="Before the attack is executed - calculate damage etc."),
	After		UMETA(DisplayName = "After", ToolTip="After the attack is executed - apply post attack effects"),
	MAX			UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EAttackStage, EAttackStage::MAX);

UENUM()
enum class EAttackRole : uint8
{
	Victim		UMETA(DisplayName = "Victim", ToolTip="The character is a victim of the attack"),
	Attacker	UMETA(DisplayName = "Attacker", ToolTip="The character is an attacker")
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttackStageExecutionDelegate, UAttackSequence*, AttackSequence);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), DisplayName="Attack Component")
class KRZYWEKARTY2_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttackComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackStage AttackStage = EAttackStage::Before;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackRole AttackRole = EAttackRole::Attacker;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category="Attack")
	FAttackStageExecutionDelegate AttackStageExecution;
};
