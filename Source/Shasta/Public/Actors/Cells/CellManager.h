// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CellManager.generated.h"

class AWorldCell;
class ACellDissolver;

/**
 * 
 */
UCLASS(
	AutoExpandCategories = ("Shasta|Cell Manager")
)
class SHASTA_API ACellManager : public AActor
{
	GENERATED_BODY()

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

public:
	//==== Methods ====

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "Shasta")
	void PlayTransition();
#endif

	UFUNCTION(CallInEditor, Category = "Shasta")
	void BeginEnemySpawn();

private:
	//==== Overrides ====
	
	void BeginPlay() override;

	//==== Methods ====

	UFUNCTION(CallInEditor, Category = "Shasta")
	void GenerateCells();
};
