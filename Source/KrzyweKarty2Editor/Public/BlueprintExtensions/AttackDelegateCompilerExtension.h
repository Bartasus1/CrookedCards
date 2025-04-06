// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintCompilerExtension.h"
#include "AttackDelegateCompilerExtension.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY2EDITOR_API UAttackDelegateCompilerExtension : public UBlueprintCompilerExtension
{
	GENERATED_BODY()

public:

	virtual void ProcessBlueprintCompiled(const FKismetCompilerContext& CompilationContext, const FBlueprintCompiledData& Data) override;
};
