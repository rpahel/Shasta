// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CellDissolver.generated.h"

class UCurveFloat;
class UCurveVector;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCellDissolverSignature);

/**
 * 
 */
UCLASS(
	AutoExpandCategories = ("Shasta|Cell Dissolver")
)
class SHASTA_API ACellDissolver : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCellDissolverSignature OnMidPointDelegate;

private:
	//==== Exposed Fields ====

	UPROPERTY(EditAnywhere, Category = "Shasta|Cell Dissolver")
	float TransitionTime = 1;

	UPROPERTY(EditAnywhere, Category = "Shasta|Cell Dissolver")
	FTransform VisibleTransform;

	UPROPERTY(EditAnywhere, Category = "Shasta|Cell Dissolver")
	FTransform HiddenTransform;

	UPROPERTY(EditAnywhere, Category = "Shasta|Cell Dissolver")
	bool bUseAdvancedCurves = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bUseAdvancedCurves", EditConditionHides), Category = "Shasta|Cell Dissolver")
	TObjectPtr<UCurveFloat> VanishAnimationCurve = nullptr;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseAdvancedCurves", EditConditionHides), Category = "Shasta|Cell Dissolver")
	TObjectPtr<UCurveVector> VanishAdvancedAnimationCurve = nullptr;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bUseAdvancedCurves", EditConditionHides), Category = "Shasta|Cell Dissolver")
	TObjectPtr<UCurveFloat> AppearAnimationCurve = nullptr;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseAdvancedCurves", EditConditionHides), Category = "Shasta|Cell Dissolver")
	TObjectPtr<UCurveVector> AppearAdvancedAnimationCurve = nullptr;

	//==== Hidden Fields ====

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Cell Dissolver|Debug")
	float CurrentTransitionTime = 0;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Cell Dissolver|Debug")
	bool bInTransitionAnimation = false;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Cell Dissolver|Debug")
	bool MidPointReached = false;

	//==== Components ===

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Pivot;

public:
	//==== Constructors ====

	ACellDissolver();

	//==== Methods ====

	void PlayTransition();
	void SetVisibilityTransform(bool IsVisible);
	bool IsInTransition() const;

private:
	//==== Overrides ====

	void Tick(float DeltaTime) override;

	//==== Methods ====

	void UpdateAnimation(float DeltaTime);
};
