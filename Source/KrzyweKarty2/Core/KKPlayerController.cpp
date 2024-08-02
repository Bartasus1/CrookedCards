// Fill out your copyright notice in the Description page of Project Settings.


#include "KKPlayerController.h"
#include "KKPlayerState.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Input/KKInputConfig.h"

#include "KrzyweKarty2/Characters/KKCharacter.h"

#include "Net/UnrealNetwork.h"

AKKPlayerController::AKKPlayerController(): PlayerInputConfig(nullptr), SelectedCharacter(nullptr)
{
	bReplicates = true;

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AKKPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false));
}

void AKKPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		for (const FInputActionTag& InputActionTag : PlayerInputConfig->InputConfig)
		{
			EnhancedInputComponent->BindAction(InputActionTag.InputAction, ETriggerEvent::Started, this, &AKKPlayerController::InputActivated, InputActionTag.InputTag);
		}
	}
}

void AKKPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AKKPlayerController, PlayerID, COND_InitialOnly);

}

AKKCharacter* AKKPlayerController::SelectCharacter()
{
	SelectedCharacter = nullptr;
	
	FHitResult HitResult;
	GetHitResultUnderCursor(CHARACTER_TRACE_CHANNEL, false, HitResult);

	if(AKKCharacter* TracedCharacter = Cast<AKKCharacter>(HitResult.GetActor()))
	{
		if(TracedCharacter->PlayerState == GetPlayerState<AKKPlayerState>())
		{
			SelectedCharacter = TracedCharacter;
		}
	}
	
	return SelectedCharacter;
}

void AKKPlayerController::InputActivated(FGameplayTag InputTag)
{
	OnInputActivated.Broadcast(InputTag);
}

void AKKPlayerController::ToggleConfirmationState(bool bEnable) const
{
	if(UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		if(bEnable)
		{
			EnhancedInputSubsystem->AddMappingContext(PlayerInputConfig->ConfirmationMappingContext, INT32_MAX);
		}
		else
		{
			EnhancedInputSubsystem->RemoveMappingContext(PlayerInputConfig->ConfirmationMappingContext);
		}
		
	}
}

