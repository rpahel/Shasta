// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/Cells/CellDissolver.h"

#include <Curves/CurveVector.h>
#include <Kismet/KismetMathLibrary.h>
#include <Logging/StructuredLog.h>

//====================================================================================
//==== PRIVATE METHODS
//====================================================================================

ACellDissolver::ACellDissolver()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACellDissolver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAnimation(DeltaTime);
}

void ACellDissolver::UpdateAnimation(float DeltaTime)
{
	if (bInTransitionAnimation)
	{
		UE_LOGFMT(LogTemp, Log, "A");

		CurrentTransitionTime += DeltaTime;

		if (CurrentTransitionTime <= TransitionTime * 0.5f)
		{
			UE_LOGFMT(LogTemp, Log, "Ba : {0}", CurrentTransitionTime);

			if (!bUseAdvancedCurves && VanishAnimationCurve)
			{
				UKismetMathLibrary::TLerp(VisibleTransform, HiddenTransform, VanishAnimationCurve->GetFloatValue(CurrentTransitionTime * 2));
			}
			else if (VanishAdvancedAnimationCurve)
			{
				SetActorTransform(FTransform(
					FQuat::Slerp(VisibleTransform.GetRotation(), HiddenTransform.GetRotation(), VanishAdvancedAnimationCurve->GetVectorValue(CurrentTransitionTime * 2).X),
					FMath::Lerp(VisibleTransform.GetLocation(), HiddenTransform.GetLocation(), VanishAdvancedAnimationCurve->GetVectorValue(CurrentTransitionTime * 2).Y),
					FMath::Lerp(VisibleTransform.GetScale3D(), HiddenTransform.GetScale3D(), VanishAdvancedAnimationCurve->GetVectorValue(CurrentTransitionTime * 2).Z)
				));
			}
			else
			{
				UE_LOGFMT(LogTemp, Log, "Ca");
				SetActorTransform(UKismetMathLibrary::TLerp(VisibleTransform, HiddenTransform, CurrentTransitionTime * 2));
			}
		}
		else
		{
			UE_LOGFMT(LogTemp, Log, "Bb : {0}", CurrentTransitionTime);

			if (!bUseAdvancedCurves && AppearAnimationCurve)
			{
				//SetRelativeTransform(FMath::Lerp(HiddenTransform, VisibleTransform, AppearAnimationCurve->GetFloatValue((CurrentTransitionTime - 0.5f) * 2)));
				UKismetMathLibrary::TLerp(HiddenTransform, VisibleTransform, AppearAnimationCurve->GetFloatValue((CurrentTransitionTime - 0.5f) * 2));
			}
			else if (AppearAdvancedAnimationCurve)
			{
				SetActorTransform(FTransform(
					FQuat::Slerp(HiddenTransform.GetRotation(), VisibleTransform.GetRotation(), AppearAdvancedAnimationCurve->GetVectorValue((CurrentTransitionTime - 0.5f) * 2).X),
					FMath::Lerp(HiddenTransform.GetLocation(), VisibleTransform.GetLocation(), AppearAdvancedAnimationCurve->GetVectorValue((CurrentTransitionTime - 0.5f) * 2).Y),
					FMath::Lerp(HiddenTransform.GetScale3D(), VisibleTransform.GetScale3D(), AppearAdvancedAnimationCurve->GetVectorValue((CurrentTransitionTime - 0.5f) * 2).Z)
				));
			}
			else
			{
				UE_LOGFMT(LogTemp, Log, "Cb");

				SetActorTransform(UKismetMathLibrary::TLerp(HiddenTransform, VisibleTransform, (CurrentTransitionTime - 0.5f) * 2));
			}
		}

		if (CurrentTransitionTime >= 1)
		{
			UE_LOGFMT(LogTemp, Log, "D");

			CurrentTransitionTime = 0;
			bInTransitionAnimation = false;
			SetActorTransform(VisibleTransform);
		}
	}
}