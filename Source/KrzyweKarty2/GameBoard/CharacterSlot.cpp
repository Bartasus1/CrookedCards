// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSlot.h"
#include "CharacterSlotStatus/CharacterSlotStatus.h"

#include "Kismet/KismetSystemLibrary.h"

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
	
	Character->SetActorRotation((GetActorForwardVector() * Character->Direction).Rotation());
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

AKKCharacter* ACharacterSlot::GetCharacterAtSlot()
{
	AKKCharacter* Character = nullptr;
	
	if(CharacterInSlotUniqueID == 0)
	{
		return Character;
	}
	
	if(AKKGameState* GameState = GetWorld()->GetGameState<AKKGameState>())
	{
		Character = GameState->GetCharacterByUniqueID(CharacterInSlotUniqueID);
	}

	return Character;
}

void ACharacterSlot::SetLocalStatus(UCharacterSlotStatus* NewLocalStatus)
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
	DOREPLIFETIME_CONDITION(ACharacterSlot, SlotAllowedDirections, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ACharacterSlot, BaseSlotRelativeDirection, COND_InitialOnly);
	DOREPLIFETIME(ACharacterSlot, CharacterInSlotUniqueID);
}

void ACharacterSlot::AddBaseSlotConnection(FRelativeDirection InputDirection, FRelativeDirection OutputDirection)
{
	BaseSlotRelativeDirection.Empty();
	
	BaseSlotRelativeDirection.Add(InputDirection);
	BaseSlotRelativeDirection.Add(OutputDirection);

	SlotAllowedDirections.Add(InputDirection); // add input direction, so it gets picked when searching for connections/links
}

bool ACharacterSlot::HasLinkInDirection(const FRelativeDirection& InDirection, FRelativeDirection& OutDirection, bool bIncludeBaseLink)
{
	bool bHasLink = false;
	FRelativeDirection NormalizedDirection = InDirection.Normalize();
	
	for (const auto& AllowedDirection : SlotAllowedDirections)
	{
		if(AllowedDirection == NormalizedDirection) // todo: BUG! we can return {2,0} which results in selecting a base slot, when we're not at the slot with allowed link to the base
		{
			OutDirection = InDirection; // fix!
			bHasLink = true;
			
			if(bIncludeBaseLink && BaseSlotRelativeDirection.IsValidIndex(0)) // should we even bother looking for base slot connection
			{
				if(BaseSlotRelativeDirection[0] == NormalizedDirection) // check if input direction matches slot's input direction for base connection
				{
					OutDirection = BaseSlotRelativeDirection[1]; // out dir is a distance to the base
				}
			}

			return bHasLink; // finish here, as we already found the connection
		}
	}

	return bHasLink;
}

