// Fill out your copyright notice in the Description page of Project Settings.


#include "BookActor.h"

#include "Components/WidgetSwitcher.h"

#include "Engine/TextureRenderTarget2D.h"

#include "Kismet/KismetRenderingLibrary.h"

#include "Slate/WidgetRenderer.h"


// Sets default values
ABookActor::ABookActor()
{
	PrimaryActorTick.bCanEverTick = false;

	BookMesh = CreateDefaultSubobject<UStaticMeshComponent>("Book Mesh");
	
}

void ABookActor::TurnNextPage()
{
	if(!PageRange.Contains(PageIndex + 3))
	{
		return;
	}

	PageIndex += 2;

	BookPageMaterials[LeftPage]->SetTextureParameterValue("PageTexture", RenderedPages[PageIndex]);
	BookPageMaterials[RightPage]->SetTextureParameterValue("PageTexture", RenderedPages[PageIndex + 1]);
}

void ABookActor::TurnPrevPage()
{
	if(!PageRange.Contains(PageIndex - 2))
	{
		return;
	}

	PageIndex -= 2;

	BookPageMaterials[LeftPage]->SetTextureParameterValue("PageTexture", RenderedPages[PageIndex]);
	BookPageMaterials[RightPage]->SetTextureParameterValue("PageTexture", RenderedPages[PageIndex + 1]);
}

void ABookActor::OpenPage(uint8 PageNumber)
{
	if(!PageRange.Contains(PageNumber))
	{
		return;
	}
	
	PageIndex = FMath::Floor(PageNumber / 2); //  ->  42 / 2 = 21; 43 / 2 = 21;

	BookPageMaterials[LeftPage]->SetTextureParameterValue("PageTexture", RenderedPages[PageIndex]);
	BookPageMaterials[RightPage]->SetTextureParameterValue("PageTexture", RenderedPages[PageIndex + 1]);
	
}

void ABookActor::BeginPlay()
{
	Super::BeginPlay();
	
	for (uint8 Index = EPageType::LeftPage; Index <= EPageType::RightPage ; Index++)
	{
		BookPageMaterials.Add(BookMesh->CreateAndSetMaterialInstanceDynamic(Index));
	}

	// In child BP, create widget with Widget Switcher and call RenderWidgetToPages()
}

void ABookActor::RenderWidgetToPages(const UWidgetSwitcher* WidgetSwitcher)
{
	const FVector2D DrawSize(2048.f);
	const int32 NumPages = (WidgetSwitcher->GetChildrenCount() + 1) & ~1; // round up to the even number
	FWidgetRenderer* WidgetRenderer = new FWidgetRenderer(false, false);
	
	RenderedPages.Init(nullptr, NumPages);
	PageRange.SetUpperBoundValue(NumPages);
	
	for (int32 i = 0; i < NumPages; i++)
	{
		UTextureRenderTarget2D* PageRenderTarget = NewObject<UTextureRenderTarget2D>(this, "RenderTarget" + i);
		PageRenderTarget->RenderTargetFormat = RTF_RGBA8;
		PageRenderTarget->ClearColor = PageDefaultColor;
		PageRenderTarget->InitAutoFormat(DrawSize.X, DrawSize.Y);
		PageRenderTarget->UpdateResourceImmediate(true);

		RenderedPages[i] = PageRenderTarget;

		if(PageDefaultMaterial) // first render default material, so that transparent fragments have page color
		{
			UKismetRenderingLibrary::DrawMaterialToRenderTarget(this, PageRenderTarget, PageDefaultMaterial);
		}

		if(UWidget* PageWidget = WidgetSwitcher->GetWidgetAtIndex(i)) // next render the widget
		{
			WidgetRenderer->DrawWidget(PageRenderTarget, PageWidget->TakeWidget(), DrawSize, 0.f);
		}
		
		
		FlushRenderingCommands();
	}
	
	BeginCleanup(WidgetRenderer);
}