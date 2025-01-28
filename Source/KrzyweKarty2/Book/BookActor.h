// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BookActor.generated.h"

class UWidgetSwitcher;
class UTextureRenderTarget2DArray;

UENUM()
enum EPageType 
{
	LeftPage,
	RightPage,
	RightPage_Turning,
	LeftPage_Turning
};

UCLASS()
class KRZYWEKARTY2_API ABookActor : public AActor
{
	GENERATED_BODY()

public:
	ABookActor();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> BookMesh; // change it to SkeletalMesh later

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Page Settings")
	FLinearColor PageDefaultColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Page Settings")
	UMaterialInterface* PageDefaultMaterial;

	UFUNCTION(BlueprintCallable)
	void TurnNextPage();

	UFUNCTION(BlueprintCallable)
	void TurnPrevPage();

	UFUNCTION(BlueprintCallable)
	void OpenPage(uint8 PageNumber);
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void RenderWidgetToPages(const UWidgetSwitcher* WidgetSwitcher);

	UPROPERTY()
	TArray<UTextureRenderTarget2D*> RenderedPages;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> BookPageMaterials;

	int32 PageIndex = 0;
	FInt32Range PageRange = FInt32Range(0);
};
