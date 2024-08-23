// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/ModifierSelector.h"

AModifierSelector::AModifierSelector()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AModifierSelector::SetOwnerActor(AActor* InOwner)
{
	OwnerActor = InOwner;
}

void AModifierSelector::Open()
{
	bIsOpen = true;
	bInAnimation = true;
	AnimationProgress = 0;
	SetActorHiddenInGame(false);
	OnOpenDelegate.Broadcast();
}

void AModifierSelector::Close()
{
	bIsOpen = false;
	bInAnimation = true;
	AnimationProgress = 0;
	CloseAnimationStartPos = GetActorLocation();
	OnCloseDelegate.Broadcast();
}

void AModifierSelector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!OwnerActor)
		return;

	const FVector myPos = GetActorLocation();
	const FVector ownerPos = OwnerActor->GetActorLocation();
	const FVector dir = (ownerPos - myPos).GetSafeNormal();
	const float sizeSqrd = (ownerPos - myPos).SizeSquared();

	SetActorRotation(dir.ToOrientationQuat());

	if(!bIsOpen && !bInAnimation)
		return;

	if (bIsOpen && !bInAnimation)
	{
		if (sizeSqrd > CloseDistance * CloseDistance)
		{
			Close();
			return;
		}
	}

	if (!bInAnimation)
		return;

	if (bIsOpen)
	{
		if (AnimationProgress >= 1)
		{
			SetActorLocation(ownerPos + OwnerActor->GetActorForwardVector() * 100);
			bInAnimation = false;
			return;
		}
		else
		{
			SetActorLocation(FMath::Lerp(ownerPos - OwnerActor->GetActorForwardVector() * 100, ownerPos + OwnerActor->GetActorForwardVector() * 100, OpenCurve->GetFloatValue(AnimationProgress)));
		}
	}
	else
	{
		if (AnimationProgress >= 1)
		{
			SetActorLocation(ownerPos - OwnerActor->GetActorForwardVector() * 100);
			SetActorHiddenInGame(true);
			bInAnimation = false;
			return;
		}
		else
		{
			SetActorLocation(FMath::Lerp(CloseAnimationStartPos, ownerPos - OwnerActor->GetActorForwardVector() * 100, OpenCurve->GetFloatValue(AnimationProgress)));
		}
	}

	AnimationProgress += DeltaTime / AnimationTime;
}

