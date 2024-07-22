// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/ABCharacterSkillAttributeSet.h"

UABCharacterSkillAttributeSet::UABCharacterSkillAttributeSet() :
	SkillRange(800.f),
	MaxSkillRange(1200.f),
	SkillAttackRate(150.f),
	MaxSkillAttackRate(300.f),
	SkillEnergy(100.f),
	MaxSkillEnergy(100.f)
{
}

void UABCharacterSkillAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetSkillRangeAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.1f, GetMaxSkillRange());
	}
	else if (Attribute == GetSkillAttackRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxSkillAttackRate());
	}
}
