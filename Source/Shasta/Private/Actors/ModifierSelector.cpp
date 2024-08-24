// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/ModifierSelector.h"
#include "Actors/Cells/CellManager.h"
#include "Kismet/GameplayStatics.h"

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
	if (CellManager->IsInTransition())
		return;

	bIsOpen = true;
	bInAnimation = true;
	AnimationProgress = 0;
	SetActorHiddenInGame(false);
	OnOpenDelegate.Broadcast();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ModifierSelectorSound.LoadSynchronous(), OwnerActor->GetActorLocation());
}

void AModifierSelector::Close()
{
	bIsOpen = false;
	bInAnimation = true;
	AnimationProgress = 0;
	CloseAnimationStartPos = GetActorLocation();
	OnCloseDelegate.Broadcast();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ModifierSelectorSound.LoadSynchronous(), GetActorLocation());
}

void AModifierSelector::Select(UPrimitiveComponent* Component)
{
	if(!Component)
		return;

	if (Component->ComponentHasTag("Mount"))
	{
		CellManager->ChangeCurrentCellTo(GetActorLocation(), "Mount");
	}
	else if (Component->ComponentHasTag("Hole"))
	{
		CellManager->ChangeCurrentCellTo(GetActorLocation(), "Hole");
	}

	Close();
}

bool AModifierSelector::IsInAnimation() const
{
	return bInAnimation;
}

void AModifierSelector::BeginPlay()
{
	Super::BeginPlay();
	GetCellManager();
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

			if(GetCellManager()->IsCurrentCellInCooldown(myPos))
				Close();

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

ACellManager* AModifierSelector::GetCellManager()
{
	if(CellManager)
		return CellManager;

	return (CellManager = Cast<ACellManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACellManager::StaticClass())));
}

