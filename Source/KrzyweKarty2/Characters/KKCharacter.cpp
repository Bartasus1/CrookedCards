﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "KKCharacter.h"
#include "AbilitySystemComponent.h"
#include "CharacterDataAsset.h"

#include "Blueprint/UserWidget.h"
#include "Engine/TextureRenderTarget2D.h"

#include "Kismet/KismetSystemLibrary.h"

#include "KrzyweKarty2/KKCharacterDeveloperSettings.h"
#include "KrzyweKarty2/AbilitySystem/Abilities/KKGameplayAbility.h"
#include "KrzyweKarty2/Core/KKPlayerState.h"
#include "KrzyweKarty2/Core/KKGameState.h"
#include "KrzyweKarty2/UI/CharacterWidget.h"
#include "Net/UnrealNetwork.h"
#include "Slate/WidgetRenderer.h"


AKKCharacter::AKKCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicatingMovement(true);

	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>("CardMesh");
	SetRootComponent(CardMesh);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UKKAttributeSet>("CharacterAttributeSet");
}

UAbilitySystemComponent* AKKCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AKKCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(CharacterDataAsset); // all characters should have valid DataAsset at BeginPlay

	AttributeSet->InitFromCharacterStatistics(CharacterDataAsset->CharacterStats);
	GetAbilitySystemComponent()->AddSpawnedAttribute(AttributeSet);

	CharacterWidget = CreateWidget<UCharacterWidget>(GetWorld(), UKKCharacterDeveloperSettings::Get()->CardWidgetClass.LoadSynchronous());
	CharacterWidget->SetCharacter(this);
	
	UpdateCharacterWidgetRender();

	if(AKKGameState* GameState = GetWorld()->GetGameState<AKKGameState>())
	{
		GameState->RegisterCharacter(this);
	}

	AbilitySystemComponent->AbilityFailedCallbacks.AddUObject(this, &AKKCharacter::PrintAbilityFailure);
}

void AKKCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateCharacterWidgetRender(DeltaSeconds);
}

void AKKCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if(CharacterDataAsset == nullptr)
	{
		return;
	}

	CardMesh->SetStaticMesh(UKKCharacterDeveloperSettings::Get()->CardMesh.LoadSynchronous());
	
	if(UMaterialInstanceDynamic* DynamicCardMaterial = CardMesh->CreateAndSetMaterialInstanceDynamic(0))
	{
		DynamicCardMaterial->SetTextureParameterValue(FName("CharacterTexture"), CharacterDataAsset->CharacterCardTexture.LoadSynchronous());
	}
}

void AKKCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AKKCharacter, CharacterID, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AKKCharacter, Direction, COND_InitialOnly);

	DOREPLIFETIME(AKKCharacter, CharacterActions);
	DOREPLIFETIME(AKKCharacter, CharacterSlotID);
	DOREPLIFETIME(AKKCharacter, PlayerState);
	DOREPLIFETIME(AKKCharacter, CharacterAbilityHandles);
}

TArray<uint8> AKKCharacter::GetSlotsForCharacterSpawn() const
{
	TArray<uint8> CharacterSlots;
	const uint8 RowSize = 20;
	
	for(uint8 i = 1; i <= RowSize; i++)
	{
		CharacterSlots.Add((Direction == 1) ? i : 21 - i); // 1,2,3,4 or 20,19,18,17
	}
	
	return CharacterSlots;
}

TArray<FRelativeDirection> AKKCharacter::GetDirectionsForMovement() const
{
	TArray<FRelativeDirection> MovementDirections;
	const int32 MovementRange = 1;
	
	for(int32 i = -MovementRange; i <= MovementRange; i++)
	{
		if(i == 0)
			continue; // skip {0, 0} - self
		
		MovementDirections.Add({Direction * i, 0}); //cross pattern
		MovementDirections.Add({0, Direction * i});
	}
	MovementDirections.Add({Direction * 2, 0}); // temp, move to Kawalerzysta
	
	return MovementDirections;
}

TArray<FRelativeDirection> AKKCharacter::GetDirectionsForDefaultAttack() const
{
	TArray<FRelativeDirection> AttackDirections;
	const int32 AttackRange = CharacterDataAsset->CharacterStats.MaxAttackRange;
	
	for(int32 i = -AttackRange; i <= AttackRange; i++)
	{
		if(i == 0)
			continue; // skip {0, 0} - self
		
		AttackDirections.Add({Direction * i, 0}); //cross pattern
		AttackDirections.Add({0, Direction * i});
	}
	
	return AttackDirections;
}

void AKKCharacter::CancelAllAbilities()
{
	AbilitySystemComponent->CancelAllAbilities();
}

void AKKCharacter::ApplyDefaultAttackToSelf(AKKCharacter* AttackSource, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(AttackSource);
	EffectContext.AddInstigator(AttackSource, AttackSource);

	AbilitySystemComponent->ApplyGameplayEffectToSelf(GameplayEffectClass->GetDefaultObject<UGameplayEffect>(), 1.f, EffectContext);

	UpdateCharacterWidgetRender();
}

// ABILITY SYSTEM COMPONENT INITIALIZATION //
void AKKCharacter::OnRep_PlayerState() // Client
{
	GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerState, this);
}

void AKKCharacter::SetPlayerState(AKKPlayerState* NewPlayerState) // Server
{
	PlayerState = NewPlayerState;
	GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerState, this);
	
	for (const TSubclassOf<UGameplayAbility> Ability : StartupAbilities) // remove later - dont set startup abilities per actor - move it to the singleton container - DeveloperSettings
	{
		GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(Ability, 1));
	}

	for (uint8 i = 0; i < CharacterDataAsset->ActiveAbilities.Num(); i++) // give character ability with different levels and store a handle to it
	{
		FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(CharacterDataAsset->CharacterAbilityClass, i));
		CharacterAbilityHandles.EmplaceAt(i, SpecHandle);
	}
	
}

// CHARACTER WIDGET UPDATES //
void AKKCharacter::UpdateCharacterWidgetRender(float DeltaTime)
{
	if(!CharacterWidget)
	{
		return;
	}
	
	CharacterWidget->UpdateAllAttributeWidgets();

	FWidgetRenderer* WidgetRenderer = new FWidgetRenderer(false, false);
	WidgetRenderer->DrawWidget(GetWidgetRenderTarget(), CharacterWidget->TakeWidget(), DrawSize, DeltaTime);

	UMaterialInstanceDynamic* DynamicCardMaterial =  CardMesh->CreateAndSetMaterialInstanceDynamic(0);
	DynamicCardMaterial->SetTextureParameterValue(FName("CharacterTexture"), GetWidgetRenderTarget());

	FlushRenderingCommands();
	BeginCleanup(WidgetRenderer);
}

UTextureRenderTarget2D* AKKCharacter::GetWidgetRenderTarget()
{
	if(!TextureRenderTarget2D)
	{
		TextureRenderTarget2D = NewObject<UTextureRenderTarget2D>(this, "RenderTarget");
		TextureRenderTarget2D->RenderTargetFormat = RTF_RGBA8;
		TextureRenderTarget2D->InitAutoFormat(DrawSize.X, DrawSize.Y);
		TextureRenderTarget2D->UpdateResourceImmediate(true);
	}

	return TextureRenderTarget2D;
}

void AKKCharacter::PrintAbilityFailure(const UGameplayAbility* GameplayAbility, const FGameplayTagContainer& GameplayTags)
{
	UKismetSystemLibrary::PrintString(this, "Ability failed: " + GameplayAbility->GetName());
	for (const FGameplayTag& Tag : GameplayTags)
	{
		UKismetSystemLibrary::PrintString(this, Tag.ToString());
	}
}
