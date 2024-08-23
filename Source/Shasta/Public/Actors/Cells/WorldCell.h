// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldCell.generated.h"

class UCellModifiersDataAsset;
class UPathComponent;
class ACellModifier;
class AEnemy;
enum class EShastaPathType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWorldCellSelfSignature, AWorldCell*, OutWorldCell);

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

public:

	FWorldCellSelfSignature OnRequestChangeDelegate;

private:
	//==== Exposed Fields ====

	UPROPERTY(EditInstanceOnly, Category = "Shasta|World Cell")
	ECellType CellType = ECellType::None;

	UPROPERTY(EditAnywhere, Category = "Shasta|World Cell")
	float CellRadius = 10000;

	UPROPERTY(EditAnywhere, Category = "Shasta|World Cell")
	TObjectPtr<UCellModifiersDataAsset> CellModifiersDataAsset;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "CellType == ECellType::Defense", EditConditionHides), Category = "Shasta|World Cell")
	FName DefenseModifierName = "Default";

	UPROPERTY(EditAnywhere, meta = (EditCondition = "CellType == ECellType::Border", EditConditionHides), Category = "Shasta|World Cell")
	bool bEnemySpawnPoint = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "CellType == ECellType::Border", EditConditionHides), Category = "Shasta|World Cell")
	FVector2D EnemySpawnTimeMinMax = FVector2D(3, 7);

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
	FName NewDefenseModifierName = "Default";

	FTimerHandle EnemySpawnTimerHandle;
	FTimerHandle CooldownTimerHandle;
	FTimerHandle LifeTimeTimerHandle;

public:
	//==== Constructors ====

	AWorldCell();

	//==== Methods ====

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "Shasta")
	void PingNeighbors();
#endif

	UFUNCTION(CallInEditor, Category = "Shasta")
	void RotateCellModifier();

	UFUNCTION(BlueprintCallable)
	float GetCellRadius() const;

	void SetCellType(ECellType InType);
	ECellType GetCellType() const;
	int32 GetDistanceFromCenter() const;
	const TMap<FIntPoint, TObjectPtr<AWorldCell>>& GetNeighbors() const;
	void ApplyNewCellModifier();
	void ChangeCellModifier(const FName& CellModifierName, bool ForceChange = false);
	ACellModifier* GetCellModifier() const;
	void StartSpawnEnemyTimer();
	TArray<UPathComponent*> GetValidPaths(const FVector& StartPoint, EShastaPathType pathType, bool DeepSearch = false);
	AWorldCell* GetCellInDirection(const FVector& Dir);
	void SetNewDefenseModifierName(const FName& InName);
	float GetDistanceToEdgeCenter() const;
	bool IsInCooldown() const;

	UFUNCTION(CallInEditor, Category = "Shasta")
	bool RequestChange(bool ForceChange = false);

	//==== Static Methods ====

	static FIntPoint GetOppositeSector(const FIntPoint& Sector);
	static FIntPoint GetAdjacentSectorCounterClockwise(const FIntPoint& Sector);
	static FIntPoint GetAdjacentSectorClockwise(const FIntPoint& Sector);

private:
	//==== Overrides ====

	void BeginPlay() override;
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

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
