// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldCell.generated.h"

class UPathComponent;
class ACellModifier;
class AEnemy;
enum class EShastaPathType : uint8;

UENUM()
enum class ECellType : uint8
{
	None	= 0,
	Border	= 1,
	Defense	= 2,
	Center	= 4
};

UCLASS(
	AutoExpandCategories = "Shasta|World Cell"
)
class SHASTA_API AWorldCell : public AActor
{
	GENERATED_BODY()

	friend class ACellManager;

private:
	//==== Exposed Fields ====

	UPROPERTY(EditInstanceOnly, Category = "Shasta|World Cell")
	ECellType CellType = ECellType::None;

	UPROPERTY(EditAnywhere, Category = "Shasta|World Cell")
	float CellRadius = 10000;

	UPROPERTY(EditAnywhere, Category = "Shasta|World Cell")
	TMap<ECellType, TSubclassOf<ACellModifier>> CellModifiersMap;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "CellType == ECellType::Border", EditConditionHides), Category = "Shasta|World Cell")
	bool bEnemySpawnPoint = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "CellType == ECellType::Border", EditConditionHides), Category = "Shasta|World Cell")
	TSubclassOf<AEnemy> EnemyTemplate;

	//==== Hidden Fields ====

	UPROPERTY(EditAnywhere, Category = "Shasta|World Cell|Debug")
	bool bDrawDebugs = false;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|World Cell|Debug")
	TObjectPtr<ACellModifier> CurrentCellModifier;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|World Cell|Debug")
	TMap<FIntPoint, TObjectPtr<AWorldCell>> Neighbors; // Neighbors, world space, clockwise

	UPROPERTY(VisibleAnywhere, Category = "Shasta|World Cell|Debug")
	TMap<FIntPoint, FVector> SideCenters; // local space, clockwise

	UPROPERTY(VisibleAnywhere, Category = "Shasta|World Cell|Debug")
	int32 DistanceFromCenter = 0;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|World Cell|Debug")
	FVector2D EnemySpawnTimeMinMax = FVector2D(3, 7);

	FTimerHandle EnemySpawnTimerHandle;

public:
	//==== Constructors ====

	AWorldCell();

	//==== Methods ====

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "Shasta")
	void PingNeighbors();
#endif

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "Shasta")
	void RotateCellModifier();
#endif

	UFUNCTION(BlueprintCallable)
	float GetCellRadius() const;

	void SetCellType(ECellType InType);
	ECellType GetCellType() const;
	int32 GetDistanceFromCenter() const;
	const TMap<FIntPoint, TObjectPtr<AWorldCell>>& GetNeighbors() const;
	void ChangeCellModifier(ECellType InCellType);
	ACellModifier* GetCellModifier() const;

	//==== Static Methods ====

	static FIntPoint GetOppositeSector(const FIntPoint& Sector);
	static FIntPoint GetAdjacentSectorCounterClockwise(const FIntPoint& Sector);
	static FIntPoint GetAdjacentSectorClockwise(const FIntPoint& Sector);

private:
	//==== Overrides ====

	void BeginPlay() override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	//==== Methods ====

	UFUNCTION(CallInEditor, Category = "Shasta")
	void SpawnEnemy();

	TArray<UPathComponent*> GetPaths() const;
	TArray<UPathComponent*> GetPathsOfType(EShastaPathType PathType) const;
	void SetDistanceFromCenter(int Distance);
	void IntroduceAsNeighbor(AWorldCell* NeighborCell, const FIntPoint& NeighborSector);
	TArray<AWorldCell*> GenerateNeighbors(const TSubclassOf<AWorldCell>& InTemplate);
};
