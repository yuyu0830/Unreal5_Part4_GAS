// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABGASWeaponBox.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Chaos/GeometryParticlesfwd.h"

void AABGASWeaponBox::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Other, WeaponEventTag, FGameplayEventData());
}
