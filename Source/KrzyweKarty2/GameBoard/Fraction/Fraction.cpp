// Fill out your copyright notice in the Description page of Project Settings.


#include "Fraction.h"

#if WITH_EDITOR
	#include "SubobjectDataHandle.h"
	#include "SubobjectDataSubsystem.h"
#endif

#include "Components/ArrowComponent.h"

#include "KrzyweKarty2/KKCharacterDeveloperSettings.h"
#include "KrzyweKarty2/Characters/KKCharacter.h"


USpawnIndicator::USpawnIndicator()
{
	const UStaticMesh* PlatformMesh = UKKCharacterDeveloperSettings::Get()->CardMesh.LoadSynchronous();
	BoxExtent = PlatformMesh->GetBoundingBox().GetExtent();

	bWantsInitializeComponent = true;
	
}

AFraction::AFraction()
{
	PrimaryActorTick.bCanEverTick = false;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Arrow");
	SetRootComponent(ArrowComponent);
}

FFractionCharacters AFraction::SpawnFractionCharacters()
{
	FFractionCharacters FractionCharacters;

	FractionCharacters.BaseCharacter = SpawnBase();
	FractionCharacters.Characters = SpawnCharacters();

	FractionCharacters.BaseCharacter->CharacterID = FractionCharacters.Num();

	return FractionCharacters;
}

TArray<AKKCharacter*> AFraction::SpawnCharacters()
{
	TArray<AKKCharacter*> SpawnedCharacters;
	TArray<TObjectPtr<USceneComponent>> SpawnIndicators = ArrowComponent->GetAttachChildren();

	check(SpawnIndicators.Num() == CharactersToSpawn.Num())

	for(int i = 0; i < CharactersToSpawn.Num(); i++)
	{
		FVector SpawnLocation = SpawnIndicators[i]->GetComponentLocation();
		FRotator SpawnRotation = GetActorRotation();

		

		SpawnedCharacters.Add(GetWorld()->SpawnActorDeferred<AKKCharacter>(CharactersToSpawn[i], FTransform(SpawnRotation, SpawnLocation)));
		SpawnedCharacters[i]->CharacterID = i + 1;
	}
	
	return SpawnedCharacters;
}

AKKCharacter* AFraction::SpawnBase() const
{
	return GetWorld()->SpawnActorDeferred<AKKCharacter>(BaseClass, FTransform(GetActorRotation() + FRotator(0.f, 90.f, 0.f), GetActorLocation()));
}

#if WITH_EDITOR
void AFraction::RemoveComponents()
{

	const TArray<TObjectPtr<USceneComponent>> ExistingComponents = ArrowComponent->GetAttachChildren();
	
	if(CharactersToSpawn.Num() < ExistingComponents.Num())
	{
		TArray<FSubobjectDataHandle> SubobjectData;
		USubobjectDataSubsystem* DataSubsystem = USubobjectDataSubsystem::Get();

		DataSubsystem->GatherSubobjectData(this, SubobjectData);
		FSubobjectDataHandle ParentHandle = SubobjectData[0];

		TArray<FSubobjectDataHandle> ObjectsToDelete;
		for(int i = CharactersToSpawn.Num(); i < ExistingComponents.Num(); i++)
		{
			if(ExistingComponents[i] != nullptr)
			{
				ObjectsToDelete.Add(SubobjectData[i + 2]); //ExistingComponents + this + ArrowComponent;
			}
		}

		FSubobjectDataHandle Dummy;
		DataSubsystem->DeleteSubobjects(ParentHandle, ObjectsToDelete,Dummy, UBlueprint::GetBlueprintFromClass(GetClass()), true);
	}
}

void AFraction::SpawnComponents()
{
	const TArray<TObjectPtr<USceneComponent>> ExistingComponents = ArrowComponent->GetAttachChildren();
	
	for(int i = 0; i < CharactersToSpawn.Num(); i++)
	{
		if(!ExistingComponents.IsValidIndex(i)) //don't create already created component
		{
			Modify();
			
			TArray<FSubobjectDataHandle> SubobjectData;
			USubobjectDataSubsystem* DataSubsystem = USubobjectDataSubsystem::Get();
			DataSubsystem->GatherSubobjectData(this, SubobjectData);

			if(FSubobjectDataHandle ParentHandle = SubobjectData[0]; ParentHandle.IsValid())
			{
				USpawnIndicator* TemplateIndicator = NewObject<USpawnIndicator>(this, FName("SpawnIndicator", i)); // use template to set initial position 
				TemplateIndicator->SetRelativeLocation(FVector(-60.f, i * 100.f, 0.f));
				
				FAddNewSubobjectParams AddSubobjectParams;
				AddSubobjectParams.BlueprintContext = UBlueprint::GetBlueprintFromClass(GetClass());
				AddSubobjectParams.NewClass = USpawnIndicator::StaticClass();
				AddSubobjectParams.ParentHandle = ParentHandle;
				AddSubobjectParams.AssetOverride = TemplateIndicator;
				
				FText OutFailReason;
				FSubobjectDataHandle Result = DataSubsystem->AddNewSubobject(AddSubobjectParams, OutFailReason);
				
				if(Result.IsValid())
				{
					USpawnIndicator* SpawnIndicator = const_cast<USpawnIndicator*>(Result.GetData()->GetObject<USpawnIndicator>());
					
		            SpawnIndicator->AttachToComponent(ArrowComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				}
			}
		}
	}
}
#endif


void AFraction::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if WITH_EDITOR
	if(ArrowComponent->GetAttachChildren().Num() != CharactersToSpawn.Num())
	{
		RemoveComponents();
		GetWorldTimerManager().SetTimerForNextTick(this, &AFraction::SpawnComponents); // don't allow spawning the components in the same frame as OnConstruction
	}
#endif
}

void AFraction::BeginPlay()
{
	Super::BeginPlay();
	
}
