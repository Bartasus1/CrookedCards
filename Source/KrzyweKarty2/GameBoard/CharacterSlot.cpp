// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSlot.h"
#include "CharacterSlotStatus/CharacterSlotStatus.h"
#include "KrzyweKarty2/Characters/KKCharacter.h"
#include "KrzyweKarty2/Core/KKGameState.h"

#include "Net/UnrealNetwork.h"


ACharacterSlot::ACharacterSlot()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	SlotMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("SlotCollision");
	SetRootComponent(SlotMeshComponent);

	SlotMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SlotMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	SlotMeshComponent->SetRelativeScale3D(FVector(0.98));
}

void ACharacterSlot::MoveCharacter_Implementation(AKKCharacter* Character)
{
	Character->SetActorLocation(GetActorLocation()); // in child BP, update location with Timeline
}

void ACharacterSlot::AssignCharacterToSlot(AKKCharacter* Character)
{
	if(!Character)
		return;
	
	CharacterInSlotUniqueID = Character->GetCharacterUniqueID();
	Character->CharacterSlotID = CharacterSlotID;

	Character->OnCharacterDeath.AddDynamic(this, &ACharacterSlot::RemoveCharacterFromSlot);
	
	MoveCharacter(Character);
}

void ACharacterSlot::RemoveCharacterFromSlot()
{
	if(AKKCharacter* Character = GetCharacterAtSlot())
	{
		Character->OnCharacterDeath.RemoveDynamic(this, &ACharacterSlot::RemoveCharacterFromSlot);
	}
	
	CharacterInSlotUniqueID = 0;
}

bool ACharacterSlot::HasCharacterAtSlot() const
{
	return CharacterInSlotUniqueID != 0;
}

AKKCharacter* ACharacterSlot::GetCharacterAtSlot() const
{
	if(HasCharacterAtSlot())
	{
		if(AKKGameState* GameState = GetWorld()->GetGameState<AKKGameState>())
		{
			return GameState->GetCharacterByUniqueID(CharacterInSlotUniqueID);
		}
	}

	return nullptr;
}

void ACharacterSlot::SetLocalStatus(const UCharacterSlotStatus* NewLocalStatus)
{
	LocalStatus = NewLocalStatus;
	
	if(MaterialInstanceDynamic )
	{
		if(LocalStatus)
		{
			MaterialInstanceDynamic->SetVectorParameterValue("Color", LocalStatus->StatusColor);
			SlotMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			MaterialInstanceDynamic->SetVectorParameterValue("Color", FLinearColor::Transparent);
			SlotMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

const FGameplayTag& ACharacterSlot::GetLocalStatusTag() const
{
	if(LocalStatus)
	{
		return LocalStatus->StatusTag;
	}

	return FGameplayTag::EmptyTag;
}

void ACharacterSlot::BeginPlay()
{
	Super::BeginPlay();

	MaterialInstanceDynamic = SlotMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
	SetLocalStatus(nullptr);
	
}

void ACharacterSlot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACharacterSlot, CharacterSlotID, COND_InitialOnly);
	DOREPLIFETIME(ACharacterSlot, CharacterInSlotUniqueID);
}

