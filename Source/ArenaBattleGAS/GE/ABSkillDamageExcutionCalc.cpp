// Fill out your copyright notice in the Description page of Project Settings.


#include "GE/ABSkillDamageExcutionCalc.h"
#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterSkillAttributeSet.h"
#include "Attribute/ABCharacterAttributeSet.h"

void UABSkillDamageExcutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	if (SourceASC && TargetASC)
	{
		AActor* SourceActor = SourceASC->GetAvatarActor();
		AActor* TargetActor = TargetASC->GetAvatarActor();

		if (SourceActor && TargetActor)
		{
			const float MaxDamageRange = SourceASC->GetNumericAttributeBase(UABCharacterSkillAttributeSet::GetSkillRangeAttribute());
			const float MaxDamage = SourceASC->GetNumericAttributeBase(UABCharacterSkillAttributeSet::GetSkillAttackRateAttribute());

			const float Distance = FMath::Clamp(SourceActor->GetDistanceTo(TargetActor), 0.f, MaxDamageRange);
			const float InvDamageRatio = 1.f - Distance / MaxDamageRange;

			float Damage = MaxDamage * InvDamageRatio;

			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UABCharacterAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, Damage));
		}
	}
}
