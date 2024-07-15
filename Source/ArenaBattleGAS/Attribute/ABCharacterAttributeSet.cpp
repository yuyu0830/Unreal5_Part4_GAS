// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/ABCharacterAttributeSet.h"
#include "ArenaBattleGAS.h"
#include "GameplayEffectExtension.h"

UABCharacterAttributeSet::UABCharacterAttributeSet() :
	AttackRange(100.f),
	MaxAttackRange(300.f),
	AttackRadius(50.f),
	MaxAttackRadius(150.f),
	AttackRate(30.f),
	MaxAttackRate(100.f),
	MaxHealth(100.f),
	Damage(0.f)
{
	InitHealth(GetMaxHealth());
}

void UABCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	/*if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}*/

	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.f ? 0.f : NewValue;
	}
}

// bool UABCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
// {
// 	Super::PreGameplayEffectExecute(Data);
//
// 	
// }

void UABCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinHealth = 0.f;
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		ABGAS_LOG(LogABGAS, Warning, TEXT("Direct Health Access : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), MinHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		ABGAS_LOG(LogABGAS, Log, TEXT("Damage : %f"), GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinHealth, GetMaxHealth()));
		SetDamage(0.f);
	}
}

// void UABCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
// {
// 	if (Attribute == GetHealthAttribute())
// 	{
// 		ABGAS_LOG(LogABGAS, Log, TEXT("Health : %f -> %f"), OldValue, NewValue);
// 	}
// }
