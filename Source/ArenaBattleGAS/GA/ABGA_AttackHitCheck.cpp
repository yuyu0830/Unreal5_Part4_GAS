// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/ABGA_AttackHitCheck.h"
#include "ArenaBattleGAS.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GA/TA/ABTA_Trace.h"
#include "GA/AT/ABAT_Trace.h"
#include "AttributeSet.h"
#include "ArenaBattleGAS.h"
#include "Attribute/ABCharacterAttributeSet.h"
#include "Tag/ABGameplayTag.h"

UABGA_AttackHitCheck::UABGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UABGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	ABGAS_LOG(LogABGAS, Log, TEXT("%s"), *TriggerEventData->EventTag.GetTagName().ToString());
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentLevel = TriggerEventData->EventMagnitude;
	
	UABAT_Trace* AttackTraceTask = UABAT_Trace::CreateTask(this, TargetActorClass);

	AttackTraceTask->OnComplete.AddDynamic(this, &UABGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UABGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		ABGAS_LOG(LogABGAS, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		//
		// if (!SourceASC || !TargetASC)
		// {
		// 	ABGAS_LOG(LogABGAS, Error, TEXT("ASC Not found!"));
		// 	return;
		// }
		//
		
		const UABCharacterAttributeSet* SourceAttributeSet = SourceASC->GetSet<UABCharacterAttributeSet>();
		// UABCharacterAttributeSet* TargetAttributeSet = const_cast<UABCharacterAttributeSet*>(TargetASC->GetSet<UABCharacterAttributeSet>());
		//
		// if (!SourceAttributeSet || !TargetAttributeSet)
		// {
		// 	ABGAS_LOG(LogABGAS, Error, TEXT("AttributeSet Not found!"));
		// 	return;
		// }
		//
		// const float AttackDamage = SourceAttributeSet->GetAttackRate();
		// TargetAttributeSet->SetHealth(TargetAttributeSet->GetHealth() - AttackDamage);

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddHitResult(HitResult);
			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;

			TargetASC->ExecuteGameplayCue(ABTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT, CueParam);
		}

		FGameplayEffectSpecHandle BuffEffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackBuffEffect, CurrentLevel);
		if (BuffEffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, BuffEffectSpecHandle);
		}
	}

	else if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddActors(TargetDataHandle.Data[0].Get()->GetActors(), false);
			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;

			SourceASC->ExecuteGameplayCue(ABTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT, CueParam);
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
