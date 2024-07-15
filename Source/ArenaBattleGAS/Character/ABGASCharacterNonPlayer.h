// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterNonPlayer.h"
#include "AbilitySystemInterface.h"
#include "ABGASCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABGASCharacterNonPlayer : public AABCharacterNonPlayer, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AABGASCharacterNonPlayer();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UABCharacterAttributeSet> AttributeSet;
};
