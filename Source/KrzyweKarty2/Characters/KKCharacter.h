// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "KrzyweKarty2/AbilitySystem/Attributes/KKAttributeSet.h"
#include "KKCharacter.generated.h"

class AKKGameBoard;
class AKKPlayerState;
class UCharacterWidget;
class UCharacterDataAsset;
class UKKAttributeSet;
struct FGameplayEffectCustomExecutionOutput;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAbilityAction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDeath);

UCLASS()
class KRZYWEKARTY2_API AKKCharacter : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AKKCharacter();

	// COMPONENTS //
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* CardMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	UAbilitySystemComponent* AbilitySystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:

	// CHARACTER DATA //
	UPROPERTY(Transient)
	UKKAttributeSet* AttributeSet; //Cached attribute set

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Data")
	UCharacterDataAsset* CharacterDataAsset;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Character Data");
	TArray<FGameplayAbilitySpecHandle> CharacterAbilityHandles; // use abilities from client

public:
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Character Data", VisibleAnywhere)
	int32 Direction = 1;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Character Data", VisibleAnywhere)
	int32 CharacterID = 1;

	UFUNCTION(BlueprintPure)
	int32 GetCharacterUniqueID() const
	{
		return CharacterID * Direction;
	}

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Character Data")
	uint8 CharacterActions = 0;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Character Data")
	uint8 CharacterSlotID = -1; // from 0 to 21 -> one for each CharacterSlot

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FAbilityAction OnBeginAbility;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FAbilityAction OnEndAbility;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnCharacterDeath OnCharacterDeath;

	// ATTACK FUNCTIONS
	// Target
	virtual bool Target_CanBeAttacked(AKKCharacter* Attacker, const FAttackInfo& AttackInfo);

	// todo: Make less generic functions to:
	// * Allow Attacker to specify Damage for DefaultAttack
	// * Notify Target that it's gonna be attacked with *such* amount of damage etc. - look Paladin second passive ability
	
	// Attacker
	virtual void Attacker_OnAttackBegin(AKKCharacter* TargetCharacter, const FAttackInfo& AttackInfo, FGameplayEffectSpec& Spec); // Spec to update attributes
	virtual void Attacker_OnAttackEnd(AKKCharacter* TargetCharacter, const FAttackInfo& AttackInfo, FGameplayEffectCustomExecutionOutput& OutExecutionOutput); // ExecutionOutput to modify other attributes for any character after attack
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UFUNCTION(BlueprintPure)
	virtual TArray<uint8> GetSlotsForCharacterSpawn() const;
	
	UFUNCTION(BlueprintPure)
	virtual TArray<FRelativeDirection> GetDirectionsForMovement() const;
	
	UFUNCTION(BlueprintPure)
	virtual TArray<FRelativeDirection> GetDirectionsForDefaultAttack() const;

	UFUNCTION(BlueprintCallable)
	void CancelAllAbilities();

protected:

	UFUNCTION(BlueprintCallable)
	AKKGameBoard* GetGameBoard() const;

public:
	// PLAYER STATE //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing="OnRep_PlayerState")
	TObjectPtr<AKKPlayerState> PlayerState;

	UFUNCTION()
	void OnRep_PlayerState();

	void SetPlayerState(AKKPlayerState* NewPlayerState);
	

private:
	// CHARACTER WIDGET //
	const FVector2D DrawSize = {512,700};

	UPROPERTY()
	TObjectPtr<UCharacterWidget> CharacterWidget;

	UFUNCTION(BlueprintCallable)
	void UpdateCharacterWidgetRender(float DeltaTime = 0.f);
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UTextureRenderTarget2D> TextureRenderTarget2D;

	UTextureRenderTarget2D* GetWidgetRenderTarget();

	UFUNCTION()
	void PrintAbilityFailure(const UGameplayAbility* GameplayAbility, const FGameplayTagContainer& GameplayTags);

public:

	UKKAttributeSet* GetAttributeSet() const
	{
		return AttributeSet;
	}
	
	UCharacterDataAsset* GetCharacterDataAsset() const
	{
		return CharacterDataAsset;
	}

	FORCEINLINE int32 GetHealth() const
	{
		return AttributeSet->GetHealth();
	}

	FORCEINLINE int32 GetMana() const
	{
		return AttributeSet->GetMana();
	}

	FORCEINLINE int32 GetDefence() const
	{
		return AttributeSet->GetDefence();
	}

	FORCEINLINE int32 GeStrength() const
	{
		return AttributeSet->GetStrength();
	}
};
