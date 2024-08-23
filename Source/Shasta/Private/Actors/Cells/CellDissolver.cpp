// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/Cells/CellDissolver.h"

#include <Curves/CurveVector.h>
#include <Kismet/KismetMathLibrary.h>
#include <Logging/StructuredLog.h>

//====================================================================================
//==== PUBLIC METHODS
//====================================================================================

ACellDissolver::ACellDissolver()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	if (Root)
		SetRootComponent(Root);

	Pivot = CreateDefaultSubobject<USceneComponent>("Pivot");
	if (Pivot)
		Pivot->SetupAttachment(Root);
}

//====================================================================================
//==== PUBLIC METHODS
//====================================================================================

void ACellDissolver::PlayTransition()
{
	UWorld* world = GetWorld();

	if(!world)
		return;

	if(!world->IsGameWorld())
		return;

	if(!Pivot)
		return;

	Pivot->SetRelativeTransform(VisibleTransform);
	bInTransitionAnimation = true;
	CurrentTransitionTime = 0;
}

void ACellDissolver::SetVisibilityTransform(bool IsVisible)
{
	Pivot->SetRelativeTransform(IsVisible ? VisibleTransform : HiddenTransform);
}

bool ACellDissolver::IsInTransition() const
{
	return bInTransitionAnimation;
}

//====================================================================================
//==== PRIVATE METHODS
//====================================================================================

void ACellDissolver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAnimation(DeltaTime);
}

void ACellDissolver::UpdateAnimation(float DeltaTime)
{
	if (bInTransitionAnimation)
	{
		CurrentTransitionTime += DeltaTime / TransitionTime;

		if (CurrentTransitionTime <= 0.5f)
		{
			if (!bUseAdvancedCurves && VanishAnimationCurve)
			{
				Pivot->SetRelativeTransform(UKismetMathLibrary::TLerp(VisibleTransform, HiddenTransform, VanishAnimationCurve->GetFloatValue(CurrentTransitionTime * 2)));
			}
			else if (VanishAdvancedAnimationCurve)
			{
				Pivot->SetRelativeTransform(FTransform(
					FQuat::Slerp(VisibleTransform.GetRotation(), HiddenTransform.GetRotation(), VanishAdvancedAnimationCurve->GetVectorValue(CurrentTransitionTime * 2).X),
					FMath::Lerp(VisibleTransform.GetLocation(), HiddenTransform.GetLocation(), VanishAdvancedAnimationCurve->GetVectorValue(CurrentTransitionTime * 2).Y),
					FMath::Lerp(VisibleTransform.GetScale3D(), HiddenTransform.GetScale3D(), VanishAdvancedAnimationCurve->GetVectorValue(CurrentTransitionTime * 2).Z)
				));
			}
			else
			{
				Pivot->SetRelativeTransform(UKismetMathLibrary::TLerp(VisibleTransform, HiddenTransform, CurrentTransitionTime * 2));
			}
		}
		else
		{
			if (!MidPointReached)
			{
				MidPointReached = true;
				OnMidPointDelegate.Broadcast();
			}

			if (!bUseAdvancedCurves && AppearAnimationCurve)
			{
				Pivot->SetRelativeTransform(UKismetMathLibrary::TLerp(HiddenTransform, VisibleTransform, AppearAnimationCurve->GetFloatValue((CurrentTransitionTime - 0.5f) * 2)));
			}
			else if (AppearAdvancedAnimationCurve)
			{
				Pivot->SetRelativeTransform(FTransform(
					FQuat::Slerp(HiddenTransform.GetRotation(), VisibleTransform.GetRotation(), AppearAdvancedAnimationCurve->GetVectorValue((CurrentTransitionTime - 0.5f) * 2).X),
					FMath::Lerp(HiddenTransform.GetLocation(), VisibleTransform.GetLocation(), AppearAdvancedAnimationCurve->GetVectorValue((CurrentTransitionTime - 0.5f) * 2).Y),
					FMath::Lerp(HiddenTransform.GetScale3D(), VisibleTransform.GetScale3D(), AppearAdvancedAnimationCurve->GetVectorValue((CurrentTransitionTime - 0.5f) * 2).Z)
				));
			}
			else
			{
				Pivot->SetRelativeTransform(UKismetMathLibrary::TLerp(HiddenTransform, VisibleTransform, (CurrentTransitionTime - 0.5f) * 2));
			}
		}

		if (CurrentTransitionTime >= 1)
		{
			CurrentTransitionTime = 0;
			bInTransitionAnimation = false;
			MidPointReached = false;
			Pivot->SetRelativeTransform(VisibleTransform);
		}
	}
}