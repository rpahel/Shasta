// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CellManager.generated.h"

class AWorldCell;
class ACellDissolver;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCellManagerSignature);

/**
 * 
 */
UCLASS(
	BlueprintType,
	AutoExpandCategories = ("Shasta|Cell Manager")
)
class SHASTA_API ACellManager : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCellManagerSignature OnBeginEnemySpawn;

	UPROPERTY(BlueprintAssignable)
	FCellManagerSignature OnCooldownCheckTriggered;

private:
	//==== Exposed Fields ====

	UPROPERTY(EditAnywhere, Category = "Shasta|Cell Manager")
	TSubclassOf<AWorldCell> WorldCellTemplate;

	UPROPERTY(EditAnywhere, Category = "Shasta|Cell Manager")
	int32 NumberOfRings = 2;

	//==== Hidden Fields ====

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Cell Manager|Debug")
	TArray<TObjectPtr<AWorldCell>> WorldCellArray;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Cell Manager|Debug")
	TObjectPtr<ACellDissolver> CellDissolver;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Cell Manager|Debug")
	float CellDistance;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Cell Manager|Debug")
	TObjectPtr<AWorldCell> CellToChange;

public:
	//==== Methods ====

	UFUNCTION(CallInEditor, Category = "Shasta")
	void PlayTransition();

	void PlayTransition(AWorldCell* InCell);

	UFUNCTION(CallInEditor, Category = "Shasta")
	void BeginEnemySpawn();

	bool IsCurrentCellInCooldown(const FVector& InLocation) const;
	void ChangeCurrentCellTo(const FVector& InLocation, const FName& ModifierName);
	AWorldCell* GetCellAt(const FVector& InLocation) const;
	bool IsInTransition() const;

private:
	//==== Overrides ====
	
	void BeginPlay() override;

	//==== Methods ====

	UFUNCTION(CallInEditor, Category = "Shasta")
	void GenerateCells();

	//==== Event Handlers ====

	UFUNCTION()
	void OnDissolverMidPointCallback();
};
