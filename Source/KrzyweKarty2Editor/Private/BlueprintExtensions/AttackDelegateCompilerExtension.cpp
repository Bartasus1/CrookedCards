// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintExtensions/AttackDelegateCompilerExtension.h"
#include "K2Node_ComponentBoundEvent.h"

#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"

#include "KrzyweKarty2/Attack/AttackComponent.h"

void UAttackDelegateCompilerExtension::ProcessBlueprintCompiled(const FKismetCompilerContext& CompilationContext, const FBlueprintCompiledData& Data)
{
	Super::ProcessBlueprintCompiled(CompilationContext, Data);

	if (CompilationContext.ConsolidatedEventGraph == nullptr)
		return;

	for (const auto& Node : CompilationContext.ConsolidatedEventGraph->Nodes)
	{
		const UK2Node_ComponentBoundEvent* ComponentNode = Cast<UK2Node_ComponentBoundEvent>(Node);
		if (!IsValid(ComponentNode)) { continue; }
		if (!IsValid(ComponentNode->DelegateOwnerClass)) { continue; }
		if (!ComponentNode->DelegateOwnerClass->IsChildOf(UAttackComponent::StaticClass())) { continue; }
		if (const auto* SCSNode = CompilationContext.Blueprint->SimpleConstructionScript->FindSCSNode(ComponentNode->ComponentPropertyName))
		{
			if (const UAttackComponent* AttackComponent = Cast<UAttackComponent>(SCSNode->ComponentTemplate.Get()))
			{
				const bool bIsValidAttackType = AttackComponent->AttackType != 0;
				const FString Text = bIsValidAttackType ?
					FString::Printf(TEXT("Will execute when owner is the [ %s ] and in the [ %s ] attack stage"), 
						*UEnum::GetDisplayValueAsText(AttackComponent->AttackRole).ToString().ToLower(),
						*UEnum::GetDisplayValueAsText(AttackComponent->AttackStage).ToString().ToUpper())
					:
					FString::Printf(TEXT("ERROR: Attack type is not set for this component. Will not execute")); 
				
				CompilationContext.MessageLog.Note(TEXT("@@ - @@"), Node, *Text);
				
				if (auto* SourceNode = Cast<UEdGraphNode>(CompilationContext.MessageLog.FindSourceObject(Node)))
				{
					SourceNode->NodeComment.Empty();
					SourceNode->SetMakeCommentBubbleVisible(true);
					SourceNode->OnUpdateCommentText(Text);
				}
			}
		}
	}
}
