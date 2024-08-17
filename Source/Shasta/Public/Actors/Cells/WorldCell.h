// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldCell.generated.h"

class UCurveFloat;
class UCurveVector;
class UPathComponent;
class ACellModifier;
enum class EShastaPathType : uint8;

UENUM()
enum class ECellType : uint8
{
	None	= 0,
	Border	= 1,
	Defense	= 2,
	Center	= 4
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
	FDissolverShapeData DissolverShapeData;

	UPROPERTY(EditDefaultsOnly, Category = "Shasta|World Cell")
	TSubclassOf<AWorldCell> WorldCellTemplate;

	//==== Hidden Fields ====

	UPROPERTY(EditAnywhere, Category = "Shasta|World Cell|Debug")
	bool bDrawDebugs = false;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|World Cell|Debug")
	float CellRadius = 100;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|World Cell|Debug")
	TObjectPtr<ACellModifier> CurrentCellModifier;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|World Cell|Debug")
	TMap<FIntPoint, TObjectPtr<AWorldCell>> Neighbors; // Neighbors, world space, clockwise

	UPROPERTY(VisibleAnywhere, Category = "Shasta|World Cell|Debug")
	TMap<FIntPoint, FVector> SideCenters; // local space, clockwise

	UPROPERTY(VisibleAnywhere, Category = "Shasta|World Cell|Debug")
	int32 DistanceFromCenter = 0;

	//==== Components ====

	UPROPERTY(VisibleAnywhere, Category = "Shasta|World Cell|Debug")
	TObjectPtr<USceneComponent> Pivot;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetDissolverShape, Category = "Shasta|World Cell|Debug")
	TObjectPtr<UStaticMeshComponent> DissolverShape;

public:
	//==== Constructors ====

	AWorldCell();

	//==== Methods ====

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "Shasta")
	void PingNeighbors();
#endif

	UFUNCTION(CallInEditor, Category = "Shasta")
	void GenerateNeighbors();

	UFUNCTION(BlueprintCallable)
	float GetCellRadius() const;

	void Init(AWorldCell* parentCell);
	int32 GetDistanceFromCenter() const;
	const TMap<FIntPoint, TObjectPtr<AWorldCell>>& GetNeighbors() const;

	//==== Static Methods ====

	static FIntPoint GetOppositeSector(const FIntPoint& Sector);
	static FIntPoint GetAdjacentSectorCounterClockwise(const FIntPoint& Sector);
	static FIntPoint GetAdjacentSectorClockwise(const FIntPoint& Sector);

private:
	//==== Overrides ====

	void BeginPlay() override;

	//==== Methods ====

	void SetDistanceFromCenter(int Distance);
	void IntroduceAsNeighbor(AWorldCell* NeighborCell, const FIntPoint& NeighborSector);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStaticMeshComponent* GetDissolverShape() const;
};
