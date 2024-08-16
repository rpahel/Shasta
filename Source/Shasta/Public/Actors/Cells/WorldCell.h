// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldCell.generated.h"

class UCurveFloat;
class UCurveVector;
class UPathComponent;
enum class EShastaPathType;

UENUM()
enum class ECellType : uint8
{
	None	= 0,
	Border	= 1,
	Defense	= 2,
	Heart	= 4
};

USTRUCT()
struct SHASTA_API FDissolverShapeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FTransform VisibleTransform;

	UPROPERTY(EditAnywhere)
	FTransform HiddenTransform;

	UPROPERTY(EditAnywhere)
	bool bUseAdvancedCurves = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bUseAdvancedCurves", EditConditionHides))
	TObjectPtr<UCurveFloat> VanishAnimationCurve = nullptr;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseAdvancedCurves", EditConditionHides))
	TObjectPtr<UCurveVector> VanishAdvancedAnimationCurve = nullptr;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bUseAdvancedCurves", EditConditionHides))
	TObjectPtr<UCurveFloat> AppearAnimationCurve = nullptr;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseAdvancedCurves", EditConditionHides))
	TObjectPtr<UCurveVector> AppearAdvancedAnimationCurve = nullptr;
};


UCLASS(
	AutoExpandCategories = "Shasta|World Cell"
)
class SHASTA_API AWorldCell : public AActor
{
	GENERATED_BODY()

private:
	//==== Exposed Fields ====

	UPROPERTY(EditDefaultsOnly, Category = "Shasta|World Cell")
	ECellType CellType = ECellType::None;

	UPROPERTY(EditDefaultsOnly, Category = "Shasta|World Cell")
	FText CellName;

	UPROPERTY(EditDefaultsOnly, Category = "Shasta|World Cell")
	TSoftObjectPtr<UMaterialInstance> CellIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Shasta|World Cell")
	FDissolverShapeData DissolverShapeData;

	//==== Hidden Fields ====

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetDissolverShape)
	TObjectPtr<UStaticMeshComponent> DissolverShape;

	TMultiMap<EShastaPathType, TObjectPtr<UPathComponent>> Paths;

public:
	//==== Constructors ====

	AWorldCell();

protected:
	//==== Methods ====

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStaticMeshComponent* GetDissolverShape() const;
};
